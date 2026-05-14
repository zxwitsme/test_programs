# Linux 内核内存管理子系统分析 (v7.1-rc3)

基于 Linux kernel v7.1-rc3 源码 `mm/` 目录（164 个 .c 文件，约 185,000 行代码）。

---

## 一、整体架构

```
用户空间
    ↓  mmap / brk / madvise / mlock
虚拟内存管理层  (mm/mmap.c, mm/memory.c)
    ↓  缺页异常 → 物理页分配
页分配器 (Buddy System)  (mm/page_alloc.c)
    ↓  高频小对象分配
SLUB Slab 分配器  (mm/slub.c)
    ↓  内存压力时回收
内存回收  (mm/vmscan.c)
    ↑  监控与策略
DAMON / memcg / PSI  (mm/damon/, mm/memcontrol.c, mm/psi.c)
```

---

## 二、页分配器（Buddy System）

**源文件：** `mm/page_alloc.c`（~7,800 行）

### 核心机制
- **伙伴系统**：按 2 的幂次（order 0~MAX_PAGE_ORDER，默认最大 order=10 即 4MB）分配连续物理页
- **迁移类型**：按页面可移动性将空闲链表分为 5 类，减少碎片：

| 类型 | 说明 |
|------|------|
| `MIGRATE_UNMOVABLE` | 不可移动，内核自身数据 |
| `MIGRATE_MOVABLE` | 可移动，用户态匿名页/文件页 |
| `MIGRATE_RECLAIMABLE` | 可回收，inode/dentry 缓存 |
| `MIGRATE_HIGHATOMIC` | 高原子性保留，用于原子分配 |
| `MIGRATE_CMA` | CMA 专用（需 `CONFIG_CMA`） |

- **PCP（Per-CPU Pages）**：每 CPU 维护热页缓存列表，快速路径零锁竞争

### GFP 标志（`include/linux/gfp_types.h`）

| 常用标志 | 含义 |
|----------|------|
| `GFP_KERNEL` | 普通内核分配，可睡眠 |
| `GFP_ATOMIC` | 原子上下文（中断），不可睡眠 |
| `GFP_USER` | 用户页，可回收 |
| `__GFP_ZERO` | 分配后清零 |
| `__GFP_NOFAIL` | 必须成功，内核将无限重试 |
| `__GFP_NORETRY` | 失败后不重试，直接返回 NULL |
| `__GFP_MOVABLE` | 分配到可移动 zone |
| `__GFP_ACCOUNT` | 计入 memcg 配额 |

---

## 三、SLUB Slab 分配器

**源文件：** `mm/slub.c`（~9,936 行，v7.1-rc3 中唯一的 slab 实现，SLAB/SLOB 已移除）

### 三级缓存层次

```
kmalloc(size, gfp)
    ↓
percpu sheave (无锁快速路径，per-CPU 缓存数组)
    ↓ miss
barn (spinlock 保护的中级缓存)
    ↓ miss
node->list_lock (慢速路径，向伙伴系统请求新页)
```

### 调试功能（`CONFIG_SLUB_DEBUG`）

| 标志 | 说明 |
|------|------|
| `_SLAB_RED_ZONE` | 在对象首尾添加哨兵值，检测越界 |
| `_SLAB_POISON` | 释放后填充毒化模式，检测 UAF |
| `_SLAB_CONSISTENCY_CHECKS` | 每次操作校验元数据 |
| `_SLAB_HWCACHE_ALIGN` | 强制缓存行对齐 |
| `_SLAB_KASAN` | KASAN 集成 |

---

## 四、虚拟内存管理

**源文件：** `mm/mmap.c`（~3,200 行）、`mm/memory.c`（~7,587 行）

### VMA 数据结构（`include/linux/mm_types.h`）

```c
struct vm_area_struct {
    unsigned long vm_start, vm_end;          // 虚拟地址范围
    struct mm_struct *vm_mm;                 // 所属进程
    pgprot_t vm_page_prot;                   // 页保护位
    vm_flags_t vm_flags;                     // VM_READ/VM_WRITE/VM_EXEC/VM_SHARED...
    const struct vm_operations_struct *vm_ops; // 文件映射操作
};
```

### 缺页处理流程（`mm/memory.c`）

```
硬件缺页异常
    ↓
do_page_fault()
    ↓
find_vma() → 权限检查
    ↓
handle_pte_fault()
    ├─ 匿名页：do_anonymous_page() → 从伙伴系统分配
    ├─ 文件页：do_fault() → 从 page cache 读取或 I/O
    ├─ 交换页：do_swap_page() → 从 swap 设备读回
    └─ COW：do_wp_page() → 写时复制
```

### 重要特性
- **ASLR**：地址空间布局随机化（`mmap_rnd_bits`）
- **NUMA 策略**：`MPOL_BIND / MPOL_PREFERRED / MPOL_INTERLEAVE`
- **madvise 提示**：`MADV_WILLNEED / MADV_DONTNEED / MADV_HUGEPAGE / MADV_PAGEOUT`
- **userfaultfd**（`mm/userfaultfd.c`，~2,241 行）：用户空间处理缺页，支持 Live Migration

---

## 五、内存回收（Memory Reclaim）

**源文件：** `mm/vmscan.c`（~8,068 行）、`mm/swap.c`（~1,156 行）

### LRU 链表

```c
enum lru_list {
    LRU_INACTIVE_ANON,   // 匿名页，回收候选
    LRU_ACTIVE_ANON,     // 活跃匿名页
    LRU_INACTIVE_FILE,   // 文件页，回收候选
    LRU_ACTIVE_FILE,     // 活跃文件页
    LRU_UNEVICTABLE,     // 不可驱逐（mlock 等）
};
```

### 回收路径

| 路径 | 触发条件 | 机制 |
|------|----------|------|
| **kswapd** | zone 低于 `low watermark` | 后台异步回收 |
| **直接回收** | 分配失败，zone 低于 `min watermark` | 同步阻塞回收 |
| **shrink_slab** | 内存压力时 | 回收 inode/dentry slab 缓存 |

### Folio（新数据结构，`include/linux/mm_types.h`）

v6.0+ 引入，统一连续物理页的抽象，替代裸 `struct page` 数组：
- 大小：2^order × PAGE_SIZE（4KB 到数 GB）
- vmscan.c 中使用 705 次，swap.c 中 309 次
- 简化大页路径，减少对 `struct page` 数组的遍历开销

### LRU-Gen（多代 LRU）

v6.1 合并，将 LRU 扩展为多代（generation）模型：
- 追踪页面的最后访问时间代数
- 更精确地区分热/冷页面
- 减少无效扫描，提升大内存系统的回收效率

---

## 六、透明大页（THP）

**源文件：** `mm/huge_memory.c`（~5,093 行）

### 工作原理

```
普通 4KB 页运行一段时间
    ↓ khugepaged 扫描
对齐且连续的 512 个 4KB 页 → 合并为 1 个 2MB PMD 大页
    ↓ 内存压力时
deferred_split → 按需拆分回 4KB
```

### 配置（`/sys/kernel/mm/transparent_hugepage/`）
- `enabled`：`always`（全局启用）/ `madvise`（需显式 `MADV_HUGEPAGE`）/ `never`
- `defrag`：碎片整理策略
- `khugepaged/scan_sleep_millisecs`：扫描间隔

---

## 七、内存压缩与迁移

| 模块 | 源文件 | 行数 | 功能 |
|------|--------|------|------|
| Compaction | `mm/compaction.c` | ~3,351 | 移动已用页，腾出高阶连续空间 |
| Migration | `mm/migrate.c` | ~2,768 | 物理页内容迁移（支持匿名/文件/大页） |

**触发场景：**
- 高阶分配失败（THP、HugePage、DMA 等）
- NUMA 热数据迁移到更近内存节点
- 内存热插拔移除节点
- CMA 分配冲突解决

---

## 八、DAMON——数据访问监控

**源文件：** `mm/damon/`（11 个文件，~11,541 行）

### 组件

| 文件 | 功能 |
|------|------|
| `core.c` | 核心框架，监控上下文/目标/区域管理 |
| `vaddr.c` | 虚拟地址空间监控 |
| `paddr.c` | 物理地址空间监控 |
| `reclaim.c` | 冷页主动回收模块 |
| `lru_sort.c` | LRU 优先级排序 |
| `sysfs.c` | sysfs 配置接口 |

### DAMOS 操作方案

| 操作 | 说明 |
|------|------|
| `DAMOS_PAGEOUT` | 将冷页换出到 swap |
| `DAMOS_HUGEPAGE` | 将热区域晋升为大页 |
| `DAMOS_NOHUGEPAGE` | 拆分冷区域的大页 |
| `DAMOS_MIGRATE_HOT/COLD` | 热/冷数据跨 NUMA 节点迁移 |
| `DAMOS_LRU_PRIO/DEPRIO` | 调整 LRU 中的页面优先级 |
| `DAMOS_STAT` | 纯统计，不操作内存 |

---

## 九、内核同页合并（KSM）

**源文件：** `mm/ksm.c`（~4,021 行）

### 算法
1. ksmd 后台线程扫描进程 VMA
2. 对页内容计算 xxhash，与 Unstable Tree 比较
3. 内容相同 → 写保护合并，迁入 Stable Tree
4. 节省内存：每个 KSM 页面只保留一份物理拷贝，多个 PTE 指向同一页

**适用场景：** KVM 虚拟化宿主机（相同 OS 镜像内存去重）、容器部署

---

## 十、连续内存分配（CMA）

**源文件：** `mm/cma.c`（~1,146 行）

- 启动时预留连续物理内存区域，平时供内核可移动页使用
- 设备需要时（DMA/GPU）将占用页迁移出去，返回连续页
- 支持多个独立区域（`cma_areas[MAX_CMA_AREAS]`）
- 内核参数：`cma=256M` 全局预留，`cma_name@start-end` 指定范围

---

## 十一、内存控制组（Memcg）

**源文件：** `mm/memcontrol.c`（~5,958 行）+ `mm/memcontrol-v1.c`（~2,259 行，cgroup v1 兼容）

### 核心限制接口（cgroup v2）

| 文件 | 说明 |
|------|------|
| `memory.max` | 硬限制，超过触发 OOM |
| `memory.high` | 软限制，超过触发回收但不 OOM |
| `memory.min` | 保护下限，不被全局回收抢走 |
| `memory.low` | 软保护，尽量不被回收 |
| `memory.swap.max` | swap 用量上限 |
| `memory.stat` | 详细统计（RSS/cache/swap 等） |
| `memory.pressure` | PSI 压力信号 |

---

## 十二、特殊内存机制

| 特性 | 源文件 | 功能 |
|------|--------|------|
| **memfd** | `mm/memfd.c`（~523 行） | 匿名文件 + 内存密封（sealing），安全共享缓冲区 |
| **userfaultfd** | `mm/userfaultfd.c`（~2,241 行） | 用户空间缺页处理，用于 Live Migration |
| **Balloon** | `mm/balloon.c`（~407 行） | 虚拟机动态内存伸缩（Virtio/Hyper-V/XEN） |
| **HugeTLB** | `mm/hugetlb.c` | 静态大页（2MB/1GB），需预先分配 |
| **page_owner** | `mm/page_owner.c` | 追踪每个物理页的分配调用栈（调试用） |
| **kmemleak** | `mm/kmemleak.c` | 内核内存泄漏检测 |

---

## 十三、关键源文件速查

| 文件 | 行数 | 功能 |
|------|------|------|
| `mm/page_alloc.c` | ~7,800 | 伙伴系统，核心页分配器 |
| `mm/slub.c` | ~9,936 | SLUB slab 分配器 |
| `mm/vmscan.c` | ~8,068 | kswapd，LRU 回收，shrinker |
| `mm/memory.c` | ~7,587 | 虚拟内存、缺页处理、COW |
| `mm/memcontrol.c` | ~5,958 | 内存 cgroup（v2） |
| `mm/huge_memory.c` | ~5,093 | 透明大页 THP |
| `mm/ksm.c` | ~4,021 | 内核同页合并 |
| `mm/compaction.c` | ~3,351 | 内存碎片压缩 |
| `mm/mmap.c` | ~3,200 | mmap / VMA 管理 |
| `mm/migrate.c` | ~2,768 | 物理页迁移 |
| `mm/userfaultfd.c` | ~2,241 | 用户态缺页处理 |
| `mm/damon/` (合计) | ~11,541 | 数据访问监控框架 |
