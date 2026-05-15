# Linux 内核 BPF 子系统分析 (v7.1-rc3)

基于 Linux kernel v7.1-rc3 源码 `kernel/bpf/`（75 个文件，**86,379 行**代码）。

---

## 一、整体架构

```
用户空间
  bpf() syscall  →  syscall.c (6,614 行)
       ↓
  验证器 verifier.c (20,203 行)
  ├─ 指令格式 / CFG 检查
  ├─ 类型推导 + 寄存器状态追踪
  └─ 内存访问 / helper 参数验证
       ↓
  fixups.c  →  dead code 消除 / 常数折叠
       ↓
  core.c (3,505 行)
  ├─ JIT 编译  (x86-64 / ARM64 / RISC-V)
  └─ 解释器执行 (fallback)
       ↓
  挂载点：kprobe / tracepoint / XDP / cgroup / LSM / struct_ops ...
```

---

## 二、BPF 程序类型（32 种）

`include/uapi/linux/bpf.h`，`enum bpf_prog_type`

| 类型 | 挂载点 | 典型用途 |
|------|--------|----------|
| `SOCKET_FILTER` | Socket recv | 包过滤（经典 BPF 起点） |
| `KPROBE` | 内核函数 | 动态追踪、性能分析 |
| `TRACEPOINT` | 静态追踪点 | 内核事件采样 |
| `RAW_TRACEPOINT` | 同上（无转换） | 高性能追踪，零拷贝 |
| `TRACING` | fentry/fexit/fmod_ret | 内核函数拦截/替换 |
| `XDP` | 网卡 RX 路径 | 高速包处理（DPDK 替代） |
| `SCHED_CLS` | tc qdisc | 流量分类与改写 |
| `SCHED_ACT` | tc action | 流量操作 |
| `SOCK_OPS` | TCP 状态机 | 连接参数调优 |
| `SK_SKB` | Sockmap | Socket 间零拷贝转发 |
| `SK_MSG` | Sockmap | 消息级路由 |
| `SK_LOOKUP` | Socket 查找 | 自定义 socket 绑定逻辑 |
| `CGROUP_SKB` | cgroup 网络 | 容器网络策略 |
| `CGROUP_SOCK` | cgroup socket | 容器 socket 权限 |
| `CGROUP_SOCK_ADDR` | cgroup connect | 透明代理、地址改写 |
| `CGROUP_SYSCTL` | cgroup sysctl | 容器 sysctl 隔离 |
| `CGROUP_SOCKOPT` | cgroup sockopt | socket 选项拦截 |
| `CGROUP_DEVICE` | cgroup 设备 | 设备访问控制 |
| `LSM` | LSM hooks | 安全策略（KRSI） |
| `STRUCT_OPS` | 内核 ops 表 | 实现 TCP 拥塞控制等 |
| `EXT` | BPF 程序 | 扩展/替换已有 BPF 程序 |
| `PERF_EVENT` | perf_event | 性能采样 |
| `FLOW_DISSECTOR` | 协议解析 | 自定义报文流解析 |
| `SK_REUSEPORT` | 端口复用 | 自定义负载均衡 |
| `NETFILTER` | Netfilter hook | 替代 iptables 规则 |
| `SYSCALL` | 用户空间触发 | 可调用系统调用的 BPF |
| `LWT_*` (3种) | 轻量级隧道 | 自定义封装/解封 |
| `LIRC_MODE2` | 红外接收 | 红外遥控处理 |

---

## 三、BPF Map 类型（35 种）

`include/uapi/linux/bpf.h`，`enum bpf_map_type`

### 基础存储
| 类型 | 结构 | 说明 |
|------|------|------|
| `HASH` | 哈希表 | 通用 KV，支持任意 key/value |
| `ARRAY` | 数组 | 固定大小，key=index，访问最快 |
| `PERCPU_HASH` | per-CPU 哈希 | 每 CPU 独立副本，无锁统计 |
| `PERCPU_ARRAY` | per-CPU 数组 | 同上，数组版本 |
| `LRU_HASH` | LRU 哈希 | 自动淘汰最旧条目 |
| `LPM_TRIE` | 前缀树 | 路由最长前缀匹配 |
| `BLOOM_FILTER` | 布隆过滤器 | 概率性成员查询，节省内存 |
| `QUEUE` | 队列 | FIFO |
| `STACK` | 栈 | LIFO |

### 程序控制
| 类型 | 说明 |
|------|------|
| `PROG_ARRAY` | 存 BPF prog FD，用于 `tail_call` |
| `ARRAY_OF_MAPS` / `HASH_OF_MAPS` | Map 嵌套，动态替换内层 Map |

### 内核对象引用
| 类型 | 说明 |
|------|------|
| `PERF_EVENT_ARRAY` | perf_event 输出目标 |
| `CGROUP_ARRAY` | cgroup FD 存储 |
| `DEVMAP` / `DEVMAP_HASH` | XDP 重定向目标设备 |
| `CPUMAP` | 跨 CPU 转发队列 |
| `XSKMAP` | AF_XDP socket 映射 |
| `SOCKMAP` / `SOCKHASH` | Socket 路由表（零拷贝转发） |
| `REUSEPORT_SOCKARRAY` | 端口复用 socket 列表 |

### 本地存储（per-object 附加数据）
| 类型 | 附加对象 |
|------|----------|
| `SK_STORAGE` | 每个 socket |
| `INODE_STORAGE` | 每个 inode |
| `TASK_STORAGE` | 每个 task_struct |
| `CGRP_STORAGE` | 每个 cgroup（新式，替代旧版） |

### 通信与共享内存
| 类型 | 说明 |
|------|------|
| `RINGBUF` | 内核→用户无锁环形缓冲区（**推荐**替代 perf_event_array） |
| `USER_RINGBUF` | 用户→内核方向 ring buffer |
| `ARENA` | BPF Arena 稀疏共享内存（v7.x 新特性） |

### 高级
| 类型 | 说明 |
|------|------|
| `STACK_TRACE` | 调用栈 ID 存储 |
| `STRUCT_OPS` | 内核结构体操作表 |
| `INSN_ARRAY` | 动态指令数组 |

---

## 四、验证器（Verifier）

**源文件：** `kernel/bpf/verifier.c`（**20,203 行**，BPF 子系统最大单文件）

### 两遍验证

**第一遍：结构检查（DFS）**
- 检查程序 DAG（有向无环图），拒绝循环
- 指令数 ≤ `BPF_MAXINSNS`（100万，扩展后）
- 跳转边界合法性
- 分析长度上限 64k 条指令

**第二遍：数据流分析**
- 对每条指令追踪所有可能路径的寄存器/栈状态
- 分支数上限 1k 个

### 寄存器状态（`struct bpf_reg_state`）

每个寄存器携带：
- **类型**：`SCALAR_VALUE` / `PTR_TO_CTX` / `PTR_TO_MAP_VALUE` / `PTR_TO_STACK` / `PTR_TO_BTF_ID` 等
- **值范围**：`smin/smax`（有符号）、`umin/umax`（无符号）、`var_off`（位掩码）
- **指针偏移**：`off`，越界访问检查依赖此值

### 关键验证能力
- **内存访问边界检查**：array/map/stack 越界立即拒绝
- **空指针检查**：helper 返回值必须先判断 NULL
- **类型安全**：不同指针类型不可混用
- **特权分级**：非特权 BPF 只能用 `unprivileged_bpf_disabled=0` 时的受限子集
- **循环支持**（v5.3+）：有界循环（`bpf_loop` helper 或 for 循环计数器上限证明）

---

## 五、JIT 编译器与执行引擎

**源文件：** `kernel/bpf/core.c`（3,505 行）

### 执行方式

| 模式 | 条件 | 性能 |
|------|------|------|
| **JIT** | `net.core.bpf_jit_enable=1`（现为默认） | 本地机器码，接近原生 C |
| **解释器** | JIT 禁用或不支持的架构 | ~10x 慢于 JIT |

### JIT 支持架构
x86-64、ARM64、ARM32、RISC-V 64/32、s390、PowerPC、MIPS、LoongArch 等

### BPF 指令集
- 10 个 64 位通用寄存器（R0-R9）+ R10（只读帧指针）
- 栈大小：512 字节
- 指令格式：8 字节定长（op/dst/src/off/imm）
- 支持 32 位子寄存器操作

---

## 六、BTF（BPF Type Format）

**源文件：** `kernel/bpf/btf.c`（9,810 行）

### 核心功能
- 描述 BPF 程序和内核的 C 数据类型
- 存储在 ELF `.BTF` 段 + 运行时 `vmlinux` 镜像
- 两遍验证：第一遍收集类型对象，第二遍验证引用和无环性

### 组成
```
BTF Header
  ├─ type section：btf_type 对象数组（struct/union/enum/ptr/array/...）
  └─ string section：所有名称（首字节 \0 = 匿名）
```

### 支持的 Kind
`INT / PTR / ARRAY / STRUCT / UNION / ENUM / ENUM64 / FWD / TYPEDEF / VOLATILE / CONST / RESTRICT / FUNC / FUNC_PROTO / VAR / DATASEC / FLOAT / DECL_TAG / TYPE_TAG`

### 主要用途
- **Map 类型检查**：验证用户传入数据符合 Map 声明的 key/value 类型
- **CO-RE 重定位**：加载时根据 vmlinux BTF 修复字段偏移
- **内核 kfunc 参数验证**：替代 helper 的新型扩展机制
- **bpf_iter**：导出内核对象数据到用户空间

---

## 七、BPF CO-RE（Compile Once - Run Everywhere）

允许单个 BPF 二进制在不同内核版本上运行，无需重新编译。

### 原理
```
编译时（Clang）：
  记录每次字段访问的 BTF 类型路径
  生成重定位记录（.BTF.ext 段）

加载时（libbpf）：
  读取 vmlinux BTF（/sys/kernel/btf/vmlinux）
  查找字段在当前内核的实际偏移
  修改指令的 offset 立即数（patch BPF insn）
```

### 支持的重定位类型
- 结构体字段偏移（最常见）
- 字段大小/符号性
- 枚举值
- 类型是否存在（条件编译替代）
- `__builtin_preserve_access_index()`：编译器内置支持

---

## 八、Ring Buffer

**源文件：** `kernel/bpf/ringbuf.c`（880 行）

### 相比 perf_event_array 的优势
- **无 per-CPU 碎片**：单一共享缓冲区，消费者视图有序
- **无锁读取**：消费者/生产者计数器在独立内存页
- **零拷贝提交**：`bpf_ringbuf_reserve()` → 直接写入 → `bpf_ringbuf_submit()`
- **可丢弃**：`bpf_ringbuf_discard()` 撤销未提交记录
- **覆盖模式**：`BPF_F_RB_OVERWRITE` 自动回收旧记录

### 数据流
```
BPF 程序
  bpf_ringbuf_reserve(map, size, 0)  → 获取直写指针
  fill data...
  bpf_ringbuf_submit(ptr, 0)         → 原子发布

用户空间（libbpf ring_buffer__poll）
  mmap 消费者页（只读）
  mmap 数据/生产者页（只读）
  epoll_wait → 零拷贝读取记录
```

---

## 九、BPF Arena（v7.x 新特性）

**源文件：** `kernel/bpf/arena.c`（976 行）

### 核心概念
稀疏共享内存区域（最大 4GB），BPF 程序与用户空间进程**同时可访问同一物理页**。

### 地址映射（x86-64）
```
用户空间地址：  0x7f7d26200000
内核虚拟地址：  0xffffc90001e69000

BPF 程序访问：
  mov eax, eax           // 截取用户指针低 32 位
  mov [rax + r12 + off]  // r12 = kern_vm_start，JIT 自动生成
```

### 用途
- BPF 程序在内核态构建数据结构，用户态直接读取（无 copy）
- 实现 BPF 程序间的大块共享状态
- `bpf_arena_alloc_pages()` kfunc 按需分配页面

---

## 十、BPF LSM（KRSI）

**源文件：** `kernel/bpf/bpf_lsm.c`（450 行）

### 原理
为每个 LSM hook 生成 **弱链接 nop 函数**（`bpf_lsm_*`），BPF 程序通过 `BPF_PROG_TYPE_LSM` 附加到这些 nop 上。

### 能力
- 实现完整的 MAC（强制访问控制）策略，不修改内核
- 可审计所有安全事件（文件/网络/进程/IPC）
- 可与 SELinux/AppArmor 共存（堆叠 LSM）
- 支持 `bpf_override_return()` 拒绝操作（返回 -EPERM）

---

## 十一、BPF struct_ops

**源文件：** `kernel/bpf/bpf_struct_ops.c`（1,492 行）

### 原理
允许 BPF 程序**实现内核 ops 结构体的函数指针**，通过 Trampoline 无缝注入内核。

### 已支持的 ops 表
- `tcp_congestion_ops`：自定义 TCP 拥塞控制算法（QUIC 友好型、游戏专用型等）
- `sched_ext_ops`：BPF 可编程调度器（`SCHED_EXT`，kernel/sched/ext.c）
- 网络驱动操作表

### 工作流
```
用户填充 struct_ops Map（函数指针 → BPF 程序 FD）
  ↓
verifier 验证每个函数签名匹配
  ↓
生成 Trampoline 镜像页（≤ 8 页）
  ↓
注册到内核子系统（如 tcp_register_congestion_control）
  ↓
内核调用 ops 函数 → Trampoline → BPF 程序执行
```

---

## 十二、BPF 迭代器（bpf_iter）

**源文件：** `kernel/bpf/bpf_iter.c`（827 行）

基于 `seq_file` 接口，让 BPF 程序遍历内核对象并输出数据。

### 支持的迭代目标
| 迭代器 | 遍历对象 |
|--------|----------|
| `task` | 所有进程/线程 |
| `task_vma` | 进程的所有 VMA |
| `task_file` | 进程打开的所有文件 |
| `bpf_map` | 所有 BPF Map |
| `bpf_prog` | 所有 BPF 程序 |
| `tcp` / `tcp6` | 所有 TCP 连接 |
| `udp` / `udp6` | 所有 UDP 连接 |
| `unix` | Unix Domain Socket |
| `cgroup` | cgroup 树 |
| `kmem_cache` | slab 缓存 |

### 用途
- 按需 dump 内核状态到用户空间（替代 `/proc` 接口）
- 大规模 socket/连接表扫描（O(1) 单次遍历）
- 动态 profiling 工具

---

## 十三、BPF Helper 函数（212 个）

`include/uapi/linux/bpf.h`，`___BPF_FUNC_MAPPER` 宏

> **注意**：Helper 列表已**冻结**，新功能通过 **kfunc**（内核导出函数）扩展，稳定性保证较弱但更灵活。

### 主要分类

| 类别 | 数量 | 典型函数 |
|------|------|----------|
| **Map 操作** | 5 | `map_lookup/update/delete/push/pop_elem` |
| **网络包** | ~50 | `skb_load/store_bytes`, `l3/l4_csum_replace`, `redirect`, `xdp_adjust_*` |
| **追踪** | ~20 | `probe_read*`, `trace_printk`, `get_stackid`, `perf_event_output` |
| **进程信息** | ~10 | `get_current_pid_tgid`, `get_current_comm`, `get_current_task` |
| **时间** | 5 | `ktime_get_ns`, `ktime_get_boot_ns`, `ktime_get_tai_ns` |
| **Socket/存储** | ~15 | `sk_storage_get/delete`, `inode_storage_*`, `task_storage_*` |
| **程序控制** | 3 | `tail_call`, `loop`, `timer_*` |
| **动态指针** | 4 | `dynptr_from_mem/skb`, `dynptr_read/write/slice` |
| **Ringbuf** | 5 | `ringbuf_reserve/submit/discard/output`, `user_ringbuf_drain` |

---

## 十四、BPF Link 类型（14 种）

管理 BPF 程序与挂载点之间的生命周期绑定。

| Link 类型 | 挂载点 |
|-----------|--------|
| `RAW_TRACEPOINT` | 原始追踪点 |
| `TRACING` | kprobe/fentry/fexit |
| `CGROUP` | cgroup hooks |
| `ITER` | bpf_iter 迭代器 |
| `NETNS` | 网络命名空间 |
| `XDP` | XDP 程序 |
| `PERF_EVENT` | perf_event |
| `KPROBE_MULTI` | 批量 kprobe（一次附加多个函数） |
| `UPROBE_MULTI` | 批量 uprobe |
| `STRUCT_OPS` | struct_ops 表 |
| `NETFILTER` | Netfilter hook |
| `TCX` | TC eXtensions（v7.x，替代传统 tc BPF） |
| `NETKIT` | 虚拟网络设备 |
| `SOCKMAP` | Sockmap |

---

## 十五、关键源文件速查

| 文件 | 行数 | 功能 |
|------|------|------|
| `kernel/bpf/verifier.c` | 20,203 | 程序验证（类型/内存/控制流） |
| `kernel/bpf/btf.c` | 9,810 | BTF 类型系统 |
| `kernel/bpf/syscall.c` | 6,614 | bpf() 系统调用实现 |
| `kernel/bpf/helpers.c` | 4,928 | helper 函数实现 |
| `kernel/bpf/core.c` | 3,505 | JIT + 解释器执行引擎 |
| `kernel/bpf/fixups.c` | 2,570 | 指令修复与优化 |
| `kernel/bpf/cgroup.c` | 2,757 | cgroup BPF 支持 |
| `kernel/bpf/hashtab.c` | 2,741 | 哈希表 Map 实现 |
| `kernel/bpf/liveness.c` | 2,201 | 寄存器生命周期分析 |
| `kernel/bpf/bpf_struct_ops.c` | 1,492 | struct_ops 框架 |
| `kernel/bpf/arraymap.c` | 1,462 | 数组 Map 实现 |
| `kernel/bpf/trampoline.c` | 1,385 | Trampoline/蹦床机制 |
| `kernel/bpf/devmap.c` | 1,198 | XDP 重定向 Map |
| `kernel/bpf/arena.c` | 976 | Arena 共享内存（新） |
| `kernel/bpf/ringbuf.c` | 880 | Ring Buffer |
| `kernel/bpf/bpf_iter.c` | 827 | 迭代器框架 |
| `kernel/bpf/bpf_lsm.c` | 450 | LSM 安全钩子 |
