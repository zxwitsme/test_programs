/*
 * Linux 调度器 API 演示程序
 *
 * 演示内容：
 *   1. 查询和设置调度策略（SCHED_NORMAL/FIFO/RR/BATCH/IDLE/DEADLINE）
 *   2. SCHED_DEADLINE 参数设置
 *   3. CPU 亲和性设置
 *   4. 优先级查询
 *
 * 编译：gcc -o sched_demo sched_demo.c -lpthread
 * 运行（DEADLINE/RT 需要 root）：sudo ./sched_demo
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/sched.h>

/* SCHED_DEADLINE 参数结构（通过 sched_setattr 系统调用设置） */
struct sched_attr {
    uint32_t size;
    uint32_t sched_policy;
    uint64_t sched_flags;
    int32_t  sched_nice;
    uint32_t sched_priority;
    /* SCHED_DEADLINE 专用字段 */
    uint64_t sched_runtime;   /* 每周期允许运行时间 (ns) */
    uint64_t sched_deadline;  /* 相对截止时间 (ns) */
    uint64_t sched_period;    /* 任务周期 (ns) */
    /* SCHED_UTIL_CLAMP 字段 */
    uint32_t sched_util_min;
    uint32_t sched_util_max;
};

static int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
    return syscall(SYS_sched_setattr, pid, attr, flags);
}

static int sched_getattr(pid_t pid, struct sched_attr *attr,
                         unsigned int size, unsigned int flags)
{
    return syscall(SYS_sched_getattr, pid, attr, size, flags);
}

static const char *policy_name(int policy)
{
    switch (policy & ~SCHED_RESET_ON_FORK) {
    case SCHED_NORMAL:   return "SCHED_NORMAL";
    case SCHED_FIFO:     return "SCHED_FIFO";
    case SCHED_RR:       return "SCHED_RR";
    case SCHED_BATCH:    return "SCHED_BATCH";
    case SCHED_IDLE:     return "SCHED_IDLE";
    case SCHED_DEADLINE: return "SCHED_DEADLINE";
    /* SCHED_EXT = 7，部分系统可能没有该宏 */
    case 7:              return "SCHED_EXT";
    default:             return "UNKNOWN";
    }
}

/* 显示当前进程的调度信息 */
static void show_sched_info(const char *label)
{
    struct sched_attr attr = { .size = sizeof(attr) };
    if (sched_getattr(0, &attr, sizeof(attr), 0) < 0) {
        perror("sched_getattr");
        return;
    }

    printf("[%s]\n", label);
    printf("  policy    : %s (%u)\n", policy_name(attr.sched_policy), attr.sched_policy);
    printf("  nice      : %d\n", attr.sched_nice);
    printf("  priority  : %u\n", attr.sched_priority);
    if (attr.sched_policy == SCHED_DEADLINE) {
        printf("  runtime   : %llu us\n", (unsigned long long)(attr.sched_runtime  / 1000));
        printf("  deadline  : %llu us\n", (unsigned long long)(attr.sched_deadline / 1000));
        printf("  period    : %llu us\n", (unsigned long long)(attr.sched_period   / 1000));
    }
    printf("  util_min  : %u\n", attr.sched_util_min);
    printf("  util_max  : %u\n", attr.sched_util_max);
    printf("\n");
}

/* 演示 CPU 亲和性 */
static void demo_cpu_affinity(void)
{
    int ncpus = (int)sysconf(_SC_NPROCESSORS_ONLN);
    cpu_set_t cpuset;

    /* 获取当前亲和性 */
    CPU_ZERO(&cpuset);
    if (sched_getaffinity(0, sizeof(cpuset), &cpuset) < 0) {
        perror("sched_getaffinity");
        return;
    }

    printf("[CPU Affinity]\n");
    printf("  Online CPUs : %d\n", ncpus);
    printf("  Current mask: ");
    for (int i = 0; i < ncpus; i++) {
        if (CPU_ISSET(i, &cpuset))
            printf("%d ", i);
    }
    printf("\n");

    /* 绑定到 CPU 0 */
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == 0)
        printf("  Pinned to CPU 0\n");
    else
        printf("  Pin failed: %s\n", strerror(errno));

    /* 恢复所有 CPU */
    for (int i = 0; i < ncpus; i++)
        CPU_SET(i, &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);
    printf("\n");
}

/* 演示 SCHED_BATCH */
static void demo_sched_batch(void)
{
    struct sched_attr attr = {
        .size        = sizeof(attr),
        .sched_policy = SCHED_BATCH,
        .sched_nice  = 5,
    };

    if (sched_setattr(0, &attr, 0) < 0) {
        printf("[SCHED_BATCH] failed: %s\n\n", strerror(errno));
        return;
    }
    show_sched_info("SCHED_BATCH (nice=5, 批处理任务)");

    /* 恢复 SCHED_NORMAL */
    attr.sched_policy = SCHED_NORMAL;
    attr.sched_nice = 0;
    sched_setattr(0, &attr, 0);
}

/* 演示 SCHED_IDLE */
static void demo_sched_idle(void)
{
    struct sched_attr attr = {
        .size         = sizeof(attr),
        .sched_policy = SCHED_IDLE,
    };

    if (sched_setattr(0, &attr, 0) < 0) {
        printf("[SCHED_IDLE] failed: %s\n\n", strerror(errno));
        return;
    }
    show_sched_info("SCHED_IDLE (最低优先级，空闲才运行)");

    attr.sched_policy = SCHED_NORMAL;
    attr.sched_nice = 0;
    sched_setattr(0, &attr, 0);
}

/* 演示 SCHED_FIFO（需要 root） */
static void demo_sched_fifo(void)
{
    struct sched_attr attr = {
        .size          = sizeof(attr),
        .sched_policy  = SCHED_FIFO,
        .sched_priority = 20,  /* 实时优先级 1-99 */
    };

    if (sched_setattr(0, &attr, 0) < 0) {
        printf("[SCHED_FIFO] failed (需要 root): %s\n\n", strerror(errno));
        return;
    }
    show_sched_info("SCHED_FIFO prio=20 (实时 FIFO)");

    /* 恢复 SCHED_NORMAL */
    attr.sched_policy  = SCHED_NORMAL;
    attr.sched_priority = 0;
    attr.sched_nice    = 0;
    sched_setattr(0, &attr, 0);
}

/* 演示 SCHED_DEADLINE（需要 root） */
static void demo_sched_deadline(void)
{
    /*
     * 配置：每 10ms 周期，允许运行 2ms，截止时间 5ms
     * 带宽占用 = runtime/period = 2ms/10ms = 20%
     */
    struct sched_attr attr = {
        .size           = sizeof(attr),
        .sched_policy   = SCHED_DEADLINE,
        .sched_runtime  =  2 * 1000 * 1000,  /* 2ms */
        .sched_deadline =  5 * 1000 * 1000,  /* 5ms */
        .sched_period   = 10 * 1000 * 1000,  /* 10ms */
    };

    if (sched_setattr(0, &attr, 0) < 0) {
        printf("[SCHED_DEADLINE] failed (需要 root): %s\n\n", strerror(errno));
        return;
    }
    show_sched_info("SCHED_DEADLINE runtime=2ms deadline=5ms period=10ms");

    /* SCHED_DEADLINE 任务不能直接切换策略，需通过 sched_yield 配合或 fork */
    attr.sched_policy   = SCHED_NORMAL;
    attr.sched_runtime  = 0;
    attr.sched_deadline = 0;
    attr.sched_period   = 0;
    sched_setattr(0, &attr, 0);
}

/* 演示 util_clamp（UCLAMP，限制任务利用率范围） */
static void demo_uclamp(void)
{
    /*
     * util_min=200: 请求调度器给该任务至少 200/1024 ≈ 20% 的 CPU 利用率预算
     *               （防止高性能任务被放到小核/低频 CPU）
     * util_max=512: 限制任务最多用 512/1024 ≈ 50% 利用率
     *               （防止后台任务拉高 CPU 频率）
     */
    struct sched_attr attr = {
        .size             = sizeof(attr),
        .sched_policy     = SCHED_NORMAL,
        .sched_flags      = SCHED_FLAG_UTIL_CLAMP_MIN | SCHED_FLAG_UTIL_CLAMP_MAX,
        .sched_util_min   = 200,
        .sched_util_max   = 512,
    };

    if (sched_setattr(0, &attr, 0) < 0) {
        printf("[UCLAMP] failed: %s\n\n", strerror(errno));
        return;
    }
    show_sched_info("UCLAMP util_min=200 util_max=512");

    /* 恢复默认 */
    attr.sched_flags    = SCHED_FLAG_UTIL_CLAMP_MIN | SCHED_FLAG_UTIL_CLAMP_MAX;
    attr.sched_util_min = 0;
    attr.sched_util_max = 1024;
    sched_setattr(0, &attr, 0);
}

int main(void)
{
    printf("=== Linux 内核调度器 API 演示 ===\n\n");

    show_sched_info("初始状态");
    demo_cpu_affinity();
    demo_sched_batch();
    demo_sched_idle();
    demo_uclamp();
    demo_sched_fifo();       /* 需要 root */
    demo_sched_deadline();   /* 需要 root */

    show_sched_info("最终状态");
    return 0;
}
