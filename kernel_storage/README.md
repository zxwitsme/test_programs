# Linux 内核块设备/存储子系统分析 (v7.1-rc3)

基于 Linux kernel v7.1-rc3 源码 `block/` 目录（57 个 .c 文件，约 60,563 行代码）。

---

## 一、整体架构

```
应用层  read/write/io_uring
    ↓
VFS（page cache，文件系统）
    ↓
块层入口  blk_mq_submit_bio()
    ├─ RQoS 链（节流/延迟/成本）← blk-throttle / blk-iolatency / blk-iocost
    ├─ cgroup 计费              ← blk-cgroup
    ├─ IO 调度器               ← BFQ / mq-deadline / Kyber / none
    └─ 硬件队列（blk-mq）
         ↓
    设备驱动（NVMe / SCSI / virtio-blk ...）
         ↓
    物理介质
```

---

## 二、多队列块层（blk-mq）

**源文件：** `block/blk-mq.c`（~5,330 行）+ `block/blk-mq.h`

### 核心数据结构

```c
/* 每个 CPU 的软件队列，零锁快速路径 */
struct blk_mq_ctx {
    struct list_head    rq_lists[HCTX_MAX_TYPES]; // 待派发请求列表
    struct blk_mq_hw_ctx *hctxs[HCTX_MAX_TYPES]; // 映射的硬件队列
};

/* 硬件队列（对应物理队列/MSI-X 向量） */
struct blk_mq_hw_ctx {
    struct blk_mq_tags  *tags;         // 请求标签管理（位图）
    struct blk_mq_ctx  **ctxs;         // 绑定的软件队列数组
    atomic_t             nr_active;    // 活跃请求数
    unsigned long        state;        // BLK_MQ_S_* 状态位
};
```

### 硬件队列类型

| 类型 | 说明 |
|------|------|
| `HCTX_TYPE_DEFAULT` | 读写混合（默认） |
| `HCTX_TYPE_READ` | 专用读队列 |
| `HCTX_TYPE_POLL` | 轮询队列（绕过中断，超低延迟） |

### 请求提交流程

```
blk_mq_submit_bio()
    ├─ blk_mq_get_ctx()            // 获取当前 CPU 软件队列
    ├─ blk_mq_get_tag()            // 从位图分配请求标签
    ├─ blk_mq_bio_to_request()     // bio → request
    └─ blk_mq_run_hw_queue()       // 触发派发
         └─ blk_mq_dispatch_rq_list() // 批量提交到驱动
```

### 关键特性
- **无锁设计**：软件队列 per-CPU，热路径零锁
- **位图标签**（`sbitmap_queue`）：高效请求 ID 管理，支持批量唤醒
- **轮询模式**：`blk_mq_poll()` 绕过中断，适合 NVMe/SPDK 场景
- **批量完成**：`io_comp_batch` 减少回调开销
- **冻结机制**：热插拔、格式化时安全挂起队列

---

## 三、IO 调度器

### 3.1 BFQ（Budget Fair Queueing）——预算公平排队

**源文件：** `block/bfq-iosched.c`（~7,682 行）+ `block/bfq-wf2q.c`（~1,701 行）

**核心理念：** 按**扇区数**（而非时间片）分配预算，适应存储设备的吞吐波动。

**算法：B-WF2Q+**（加权公平队列）
- 保证各进程按权重比例获得 IOPS
- 交互式应用检测：活跃写者临时提权（减少延迟）
- 软实时应用支持（`low_latency` 模式）
- Cgroup 层级整合：权重继承，按树形分配带宽

**适用场景：** 桌面、HDD、需要严格 IO 公平性的场景

### 3.2 mq-deadline——截止时间感知

**源文件：** `block/mq-deadline.c`（~1,029 行）

**三级优先级：**

| 优先级 | 说明 | 读截止 | 写截止 |
|--------|------|--------|--------|
| `DD_RT_PRIO` | 实时（`IOPRIO_CLASS_RT`） | 500ms | 5s |
| `DD_BE_PRIO` | 最佳努力（默认） | 500ms | 5s |
| `DD_IDLE_PRIO` | 空闲 | 500ms | 5s |

- 每优先级维护 RB 树（按扇区排序）+ FIFO 列表（按时间排序）
- 优先级老化：`prio_aging_expire = 10s`，防止低优先级饥饿
- 追踪最近派发位置，避免磁头大跨度跳跃

**适用场景：** NVMe SSD、数据库、延迟敏感型工作负载

### 3.3 Kyber——延迟感知自适应

**源文件：** `block/kyber-iosched.c`（~1,033 行）

**调度域与延迟目标：**

| 域 | 目标延迟 | 队列深度 |
|----|----------|----------|
| READ | 2ms | 256 |
| WRITE | 10ms | 128 |
| DISCARD | 5s | 64 |
| OTHER | — | 16 |

- 动态调整队列深度以维持目标延迟（CoDel 风格）
- 异步请求保留 25% 容量，防止同步请求饥饿
- 极简设计，适合高速 NVMe 设备

---

## 四、IO Cgroup——块设备资源限制

**源文件：** `block/blk-cgroup.c`（~2,266 行）

### 三层限流栈（RQoS 链）

```
BIO 提交
  ↓
blk-throttle  (block/blk-throttle.c, ~1,849 行)
  → 硬限制 BPS/IOPS，超限入队等待
  ↓
blk-iolatency (block/blk-iolatency.c, ~1,068 行)
  → 延迟目标，通过缩减队列深度软性限流
  ↓
blk-iocost    (block/blk-iocost.c, ~3,562 行)
  → 虚拟时间成本模型，按权重精确分配 IO 带宽
```

### cgroup v2 接口

| 接口文件 | 格式 | 说明 |
|----------|------|------|
| `io.max` | `MAJ:MIN rbps=N wbps=N riops=N wiops=N` | 硬限制（节流器） |
| `io.weight` | `MAJ:MIN weight=N` | 权重（iocost） |
| `io.latency` | `MAJ:MIN target=Nms` | 延迟目标（iolatency） |
| `io.stat` | 只读 | 读/写/丢弃字节数和 IO 计数 |
| `io.cost.model` | 参数 | iocost 成本函数参数 |

### IO 统计（无锁设计）
- `blkg_iostat_set` 采用 per-CPU + `llist` 无锁追加
- `blkcg_rstat_flush()` 定期汇总，避免热路径锁竞争

---

## 五、BIO 层

**源文件：** `block/bio.c`（~2,003 行）

### BIO 核心结构

```c
struct bio {
    struct bvec_iter    bi_iter;           // 当前读写位置（扇区偏移 + 剩余字节）
    struct bio_vec     *bi_io_vec;         // 物理页向量数组
    struct block_device *bi_bdev;          // 目标块设备
    blk_opf_t           bi_opf;           // REQ_OP_READ/WRITE/FLUSH/DISCARD + 标志
    bio_end_io_t       *bi_end_io;         // 完成回调
    struct bio_crypt_ctx *bi_crypt_context; // 内联加密上下文（可选）
};
```

### BIO 分配缓存（per-CPU）

```c
struct bio_alloc_cache {
    struct bio *free_list;     // 普通上下文缓存
    struct bio *free_list_irq; // 中断上下文缓存
    unsigned int nr, nr_irq;   // 缓存计数
};
```
- 阈值：`ALLOC_CACHE_THRESHOLD = 16`，最大 `ALLOC_CACHE_MAX = 256`
- biovec 按大小分档 slab：16/64/128/BIO_MAX_VECS

### BIO 分裂与合并
- `bio_split()`：按设备段限制分裂超大 BIO
- `bio_try_merge_hw_page()`：合并相邻物理页，减少 DMA 段数

---

## 六、块设备加密（blk-crypto）

**源文件：** `block/blk-crypto.c`（~582 行）+ `block/blk-crypto-fallback.c`（~679 行）

### 支持的加密模式

| 模式 | 密钥长度 | IV 长度 | 说明 |
|------|----------|---------|------|
| `AES-256-XTS` | 64 字节 | 16 字节 | 主流全盘加密 |
| `AES-128-CBC-ESSIV` | 16 字节 | — | 兼容旧设备 |
| `Adiantum` | 32 字节 | — | 低端 CPU 无 AES 加速时 |
| `SM4-XTS` | 32 字节 | — | 国密标准 |

### 两条路径

```
BIO 提交
  ├─ 设备支持内联加密（ICE）→ blk_crypto_profile → 硬件密钥槽（零拷贝）
  └─ 软件回退路径 → blk_crypto_fallback → Linux crypto API（AES/SM4 软件实现）
```

- `bio_crypt_ctx_mergeable()`：确保相邻 BIO 的加密上下文兼容才能合并
- 密钥槽管理：`blk_crypto_get_keyslot()` / `blk_crypto_put_keyslot()`

---

## 七、分区块设备（Zoned Block Devices）

**源文件：** `block/blk-zoned.c`（~2,476 行）

### 分区状态机

```
EMPTY → IMP_OPEN（隐式打开）→ EXP_OPEN（显式打开）→ FULL
  ↓                                                      ↓
RESET ←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←←
```

### Zone Write Plug（写塞）机制

```c
struct blk_zone_wplug {
    struct bio_list  bio_list;   // 同区域内排队的 BIO
    struct work_struct bio_work; // 异步派发工作项
    unsigned int     wp_offset;  // 写指针偏移（扇区）
    enum blk_zone_cond cond;     // 当前分区状态
};
```

- 强制同一分区内顺序写（SMR HDD / ZNS SSD 硬件约束）
- 哈希表快速查找 zone wplug
- 支持 `ZONE_RESET / ZONE_OPEN / ZONE_CLOSE / ZONE_FINISH` 管理命令

**适用设备：** SMR 机械硬盘、ZNS NVMe SSD

---

## 八、RQoS 框架——请求服务质量

**源文件：** `block/blk-wbt.c`（~1,026 行）

### 链式架构

```c
struct rq_qos {
    const struct rq_qos_ops *ops;  // throttle/track/issue/done/exit
    struct rq_qos *next;           // 链表，串联多个策略
    enum rq_qos_id id;             // RQ_QOS_WBT / LATENCY / COST
};
```

### WBT（Write Back Throttle）

- **目标**：防止写回 IO 饿死前台读请求
- **算法**：CoDel 风格延迟监控，动态缩减写队列深度
- **窗口**：默认 `100ms / sqrt(scaling_step + 1)`
- **深度缩放**：每次超限后折半，恢复时 ×2

---

## 九、IO 统计与调试接口

### sysfs（`/sys/block/<dev>/queue/`）

| 属性 | 说明 |
|------|------|
| `scheduler` | 当前 IO 调度器，可写切换 |
| `nr_requests` | 请求队列深度 |
| `max_sectors_kb` | 单个请求最大大小 |
| `rotational` | 0=SSD，1=HDD（影响调度器行为） |
| `write_cache` | 写缓存策略 |

### debugfs（`/sys/kernel/debug/block/<dev>/`）
- `hctx_*`：每个硬件队列的详细状态
- `sched_*`：调度器内部数据
- `zone_wplugs`：分区写塞状态（仅 zoned 设备）

### 跟踪点
- `block:block_rq_insert/issue/complete`：请求全生命周期
- `block:block_bio_*`：BIO 级别跟踪

---

## 十、关键数据流

### 写请求完整路径

```
write() syscall
    ↓ VFS + page cache
blk_mq_submit_bio()
    ├─ blk_throtl_bio()         // cgroup 节流检查
    ├─ rq_qos->throttle()       // RQoS 链（WBT/latency/cost）
    ├─ blk_cgroup_bio_start()   // cgroup IO 统计
    └─ blk_mq_get_ctx()         // per-CPU 软件队列
         ↓ 分配 request + 标签
    IO 调度器 enqueue
         ↓ 触发派发
    blk_mq_dispatch_rq_list()
         ↓ 驱动 .queue_rq()
    硬件完成中断
         ↓
    blk_mq_complete_request()
         ├─ blk_stat_add()      // 延迟采样
         └─ bio_endio()         // 回调链（cgroup → RQoS → 文件系统）
```

---

## 十一、关键源文件速查

| 文件 | 行数 | 功能 |
|------|------|------|
| `block/bfq-iosched.c` | ~7,682 | BFQ 预算公平排队 IO 调度器 |
| `block/blk-mq.c` | ~5,330 | 多队列块层核心 |
| `block/blk-iocost.c` | ~3,562 | IO 成本模型限流 |
| `block/blk-zoned.c` | ~2,476 | 分区块设备（ZBD/ZNS） |
| `block/blk-cgroup.c` | ~2,266 | 块设备 cgroup 控制器 |
| `block/bio.c` | ~2,003 | BIO 分配与管理 |
| `block/blk-throttle.c` | ~1,849 | BPS/IOPS 硬限制 |
| `block/bfq-wf2q.c` | ~1,701 | BFQ B-WF2Q+ 调度算法 |
| `block/blk-sysfs.c` | ~1,071 | sysfs 接口 |
| `block/blk-iolatency.c` | ~1,068 | IO 延迟目标限流 |
| `block/mq-deadline.c` | ~1,029 | 截止时间调度器 |
| `block/kyber-iosched.c` | ~1,033 | Kyber 延迟感知调度器 |
| `block/blk-wbt.c` | ~1,026 | WBT 写回限制 |
| `block/blk-mq-debugfs.c` | ~817 | 调试接口 |
| `block/blk-crypto-fallback.c` | ~679 | 软件块加密回退 |
| `block/blk-crypto.c` | ~582 | 块层内联加密 |
