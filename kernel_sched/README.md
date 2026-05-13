# Linux 内核调度器分析 (v7.1-rc3)

基于 Linux kernel v7.1-rc3 源码 (`kernel/sched/`) 的调度器功能梳理。

---

## 一、调度器类层次结构

Linux 内核使用**调度器类 (sched_class)** 的插件化设计，各类按优先级从高到低排列：

```
stop_sched_class      ← 最高优先级，用于 stop machine / CPU 热插拔
    ↓
dl_sched_class        ← SCHED_DEADLINE（EDF 实时任务）
    ↓
rt_sched_class        ← SCHED_FIFO / SCHED_RR（软实时任务）
    ↓
ext_sched_class       ← SCHED_EXT（BPF 可扩展调度器，新特性）
    ↓
fair_sched_class      ← SCHED_NORMAL / SCHED_BATCH / SCHED_IDLE（普通任务）
    ↓
idle_sched_class      ← 最低优先级，CPU 空闲时运行
```

每个调度器类实现 `struct sched_class` 中定义的一组回调函数（enqueue/dequeue/pick_next/put_prev 等）。

---

## 二、调度策略（Scheduling Policies）

| 宏定义             | 值 | 所属调度类  | 用途说明                                      |
|--------------------|----|-------------|-----------------------------------------------|
| `SCHED_NORMAL`     | 0  | fair (EEVDF) | 默认策略，适用于普通交互式和后台任务          |
| `SCHED_FIFO`       | 1  | rt          | 实时 FIFO，不被同优先级任务抢占              |
| `SCHED_RR`         | 2  | rt          | 实时轮转，同优先级任务按时间片轮转           |
| `SCHED_BATCH`      | 3  | fair (EEVDF) | 批处理任务，降低交互性但提升吞吐             |
| `SCHED_IDLE`       | 5  | fair (EEVDF) | 极低优先级，系统空闲时才运行                 |
| `SCHED_DEADLINE`   | 6  | dl          | 硬实时，基于 EDF+CBS 算法                   |
| `SCHED_EXT`        | 7  | ext         | BPF 可编程调度器（新，v6.12+）              |

---

## 三、核心调度算法

### 3.1 EEVDF —— Earliest Eligible Virtual Deadline First

**替代了历史上的 CFS（Completely Fair Scheduler）**，自 v6.6 起正式使用。

- 每个任务维护 `vruntime`（虚拟运行时间）和 `vdeadline`（虚拟截止时间）
- 任务只有在 `lag >= 0`（已接收服务 ≤ 应得服务）时才"合格"（eligible）
- 在合格任务中选择 **虚拟截止时间最早** 的运行
- 核心公式：
  ```
  lag_i = S - s_i = w_i * (V - v_i)
  -r_max < lag < max(r_max, q)
  ```
- 相比 CFS 改善了延迟公平性，特别是 wakeup-preemption 行为

**关键特性开关（`kernel/sched/features.h`）：**

| 特性              | 默认  | 说明                                          |
|-------------------|-------|-----------------------------------------------|
| `PLACE_LAG`       | on    | 睡眠/唤醒跨越时保留 lag，避免不公平           |
| `PLACE_DEADLINE_INITIAL` | on | 新任务给半个 slice，平滑进入竞争       |
| `PLACE_REL_DEADLINE` | on | 迁移时保留相对虚拟截止时间              |
| `RUN_TO_PARITY`   | on    | 当前任务在 lag=0 前不被抢占                  |
| `PREEMPT_SHORT`   | on    | 更短 slice 的唤醒任务可取消 RUN_TO_PARITY    |
| `DELAY_DEQUEUE`   | on    | 延迟出队，让负 lag 任务留在竞争中消耗 lag    |
| `DELAY_ZERO`      | on    | 出队时将 lag 截断到 0                        |
| `WAKEUP_PREEMPTION` | on  | 允许唤醒时抢占当前任务                       |
| `NEXT_BUDDY`      | off   | 优先调度最近唤醒的任务（缓存亲和性）         |

### 3.2 SCHED_DEADLINE —— EDF + CBS

- **Earliest Deadline First (EDF)**：就绪任务中选截止时间最近的
- **Constant Bandwidth Server (CBS)**：每个任务分配 `(runtime, deadline, period)` 三元组
  - `runtime`：每个 period 内允许运行的时间（ns）
  - `deadline`：相对截止时间（ns）
  - `period`：任务周期（ns）
- 超限任务通过 CBS 节流（throttle），不影响其他任务
- `sysctl` 调节范围：`100us ~ 4s`

### 3.3 SCHED_EXT —— BPF 可扩展调度器

Linux v6.12 合并，**用 BPF 程序实现用户态可编程调度策略**：

- 用户通过 BPF 程序实现 `scx_ops`（调度器操作集）
- 支持**多调度器实例**（`CONFIG_EXT_SUB_SCHED`），不同 cgroup 可用不同策略
- 内置 watchdog 机制：BPF 调度器卡死时自动 fallback 到 SCHED_NORMAL
- 典型使用场景：
  - 游戏/桌面的延迟优先调度（scx_lavd、scx_rustland）
  - 数据中心的负载感知调度（scx_central、scx_flatcg）
  - 实验性调度算法快速验证，无需重新编译内核

---

## 四、负载追踪与频率调节

### 4.1 PELT —— Per-Entity Load Tracking

- 对每个调度实体（task/cgroup）独立追踪 `util_avg`（利用率）和 `load_avg`（负载）
- 使用**指数移动平均**，半衰期约 32ms
- 信号用于：负载均衡、EAS 能耗调度、cpufreq 调节

### 4.2 schedutil —— 基于调度器的 cpufreq 调速器

- 直接读取 PELT 的 `util_avg` 驱动 CPU 频率
- 相比 `ondemand`/`conservative` 响应更快、更精准
- 特性 `UTIL_EST`：对 wakeup 任务用估算利用率，避免频率抖动

### 4.3 PSI —— Pressure Stall Information

- 追踪任务因 CPU/内存/IO 资源不足而停滞的时间比例
- 暴露到 `/proc/pressure/{cpu,memory,io}`
- 支持阈值触发通知（用于 Android 的 LMKD 等）

---

## 五、Energy Aware Scheduling (EAS)

针对**大小核（big.LITTLE / DynamIQ）** 异构系统的能效调度：

- 依赖 `Energy Model`（每个 CPU 的功耗-频率模型）
- 任务唤醒时评估将任务放到不同 CPU 的能量消耗，选择最省能的 CPU
- 与 `schedutil` 协同工作（利用率预测 → 频率选择 → 能量估算）
- 仅在系统未过载时启用（避免能效优化牺牲性能）

---

## 六、SMP 负载均衡

- 每个 CPU 维护独立的 run queue (`struct rq`)
- 触发时机：`newidle_balance`（CPU 空闲时）、周期性 tick、任务迁移
- 以**调度域（sched_domain）**为单位做层次化均衡：
  ```
  SMT (HT siblings) → MC (同 socket 核心) → NUMA → ...
  ```
- `SIS_UTIL`：搜索空闲 CPU 时限制扫描范围，减少 LLC 缓存颠簸
- `TTWU_QUEUE`：远程唤醒任务入队到目标 CPU 的队列，通过 IPI 处理

---

## 七、其他重要特性

| 特性               | 说明                                                              |
|--------------------|-------------------------------------------------------------------|
| **Autogroup**      | 按终端 session 自动分组，桌面下防止编译任务饿死交互任务          |
| **cgroup CPU**     | 通过 cgroup v2 `cpu.weight`/`cpu.max` 限制进程组 CPU 使用       |
| **Core Scheduling** (`core_sched.c`) | 同一物理核的两个 HT 线程只运行同一安全组的任务，缓解 MDS/侧信道 |
| **CPU Isolation**  | `isolcpus=`/`cpuset` 隔离 CPU，供实时任务独占                   |
| **membarrier**     | 提供进程间内存屏障原语，减少 IPI 开销                            |
| **Thermal pressure** | 温度过高时降低 CPU 容量估算，影响任务放置                     |

---

## 八、源码文件速查

| 文件                   | 行数  | 内容                                  |
|------------------------|-------|---------------------------------------|
| `core.c`               | 11236 | 调度器核心：上下文切换、抢占、tick    |
| `fair.c`               | 14312 | EEVDF/CFS 实现、EAS、负载均衡        |
| `ext.c` + `ext_idle.c` | 11362 | BPF sched_ext 框架                   |
| `deadline.c`           | 3872  | SCHED_DEADLINE EDF+CBS               |
| `rt.c`                 | 2939  | SCHED_FIFO / SCHED_RR                |
| `topology.c`           | 3016  | 调度域构建                            |
| `pelt.c`               | ~     | Per-Entity Load Tracking             |
| `cpufreq_schedutil.c`  | 938   | schedutil 调速器                     |
| `psi.c`                | 1682  | Pressure Stall Information           |
| `features.h`           | ~130  | 调度特性开关（可通过 debugfs 修改）  |
