/*
 * Linux BPF 子系统 API 演示程序
 *
 * 演示内容（无需 root，使用受限的非特权 BPF）：
 *   1. bpf() 系统调用基础：创建并查询各类 Map
 *   2. BPF_MAP_TYPE_HASH          - 通用哈希表
 *   3. BPF_MAP_TYPE_ARRAY         - 数组 Map
 *   4. BPF_MAP_TYPE_PERCPU_ARRAY  - per-CPU 数组（原子计数器）
 *   5. BPF_MAP_TYPE_RINGBUF       - 无锁环形缓冲区
 *   6. BPF_MAP_TYPE_QUEUE / STACK - FIFO / LIFO
 *   7. BPF_MAP_TYPE_BLOOM_FILTER  - 布隆过滤器
 *   8. BPF_MAP_TYPE_LRU_HASH      - LRU 哈希表
 *   9. bpf_obj_get_info_by_fd()   - 查询 Map 元数据
 *  10. /proc/sys/kernel/unprivileged_bpf_disabled 状态查询
 *  11. btf_id / prog_type 枚举打印
 *
 * 编译：gcc -o bpf_demo bpf_demo.c -Wall -O2
 * 运行：./bpf_demo         (部分 Map 需要内核 ≥ 5.1)
 *       sudo ./bpf_demo    (解锁全部特性)
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/bpf.h>
#include <linux/unistd.h>

/* ── 最小 bpf() 系统调用封装 ── */
static int sys_bpf(enum bpf_cmd cmd, union bpf_attr *attr, unsigned int size)
{
    return (int)syscall(SYS_bpf, cmd, attr, size);
}

/* ── Map 创建辅助函数 ── */
static int map_create(enum bpf_map_type type, unsigned key_size,
                      unsigned value_size, unsigned max_entries,
                      unsigned map_flags)
{
    union bpf_attr attr = {
        .map_type    = type,
        .key_size    = key_size,
        .value_size  = value_size,
        .max_entries = max_entries,
        .map_flags   = map_flags,
    };
    return sys_bpf(BPF_MAP_CREATE, &attr, sizeof(attr));
}

static int map_update(int fd, const void *key, const void *value, uint64_t flags)
{
    union bpf_attr attr = {
        .map_fd  = (uint32_t)fd,
        .key     = (uint64_t)(uintptr_t)key,
        .value   = (uint64_t)(uintptr_t)value,
        .flags   = flags,
    };
    return sys_bpf(BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));
}

static int map_lookup(int fd, const void *key, void *value)
{
    union bpf_attr attr = {
        .map_fd  = (uint32_t)fd,
        .key     = (uint64_t)(uintptr_t)key,
        .value   = (uint64_t)(uintptr_t)value,
    };
    return sys_bpf(BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
}

static int map_delete(int fd, const void *key)
{
    union bpf_attr attr = {
        .map_fd = (uint32_t)fd,
        .key    = (uint64_t)(uintptr_t)key,
    };
    return sys_bpf(BPF_MAP_DELETE_ELEM, &attr, sizeof(attr));
}

static int map_get_next_key(int fd, const void *key, void *next_key)
{
    union bpf_attr attr = {
        .map_fd   = (uint32_t)fd,
        .key      = (uint64_t)(uintptr_t)key,
        .next_key = (uint64_t)(uintptr_t)next_key,
    };
    return sys_bpf(BPF_MAP_GET_NEXT_KEY, &attr, sizeof(attr));
}

/* ── Map 信息查询 ── */
static void print_map_info(int fd, const char *label)
{
    struct bpf_map_info info = {};
    union bpf_attr attr = {
        .info.bpf_fd   = (uint32_t)fd,
        .info.info_len = sizeof(info),
        .info.info     = (uint64_t)(uintptr_t)&info,
    };
    if (sys_bpf(BPF_OBJ_GET_INFO_BY_FD, &attr, sizeof(attr)) < 0) {
        printf("  [%s] get_info failed: %s\n", label, strerror(errno));
        return;
    }
    printf("  [%s] id=%u type=%u key_size=%u value_size=%u max_entries=%u flags=0x%x\n",
           label, info.id, info.type, info.key_size,
           info.value_size, info.max_entries, info.map_flags);
}

/* ──────────────────────────────────────────────────────
 * 1. HASH Map
 * ────────────────────────────────────────────────────── */
static void demo_hash_map(void)
{
    printf("[BPF_MAP_TYPE_HASH]\n");

    int fd = map_create(BPF_MAP_TYPE_HASH, sizeof(uint32_t),
                        sizeof(uint64_t), 1024, 0);
    if (fd < 0) { printf("  create failed: %s\n\n", strerror(errno)); return; }

    print_map_info(fd, "hash");

    /* 写入 5 个键值对 */
    for (uint32_t i = 1; i <= 5; i++) {
        uint64_t val = (uint64_t)i * 100;
        map_update(fd, &i, &val, BPF_ANY);
    }

    /* 查询 */
    uint32_t key = 3;
    uint64_t val = 0;
    if (map_lookup(fd, &key, &val) == 0)
        printf("  lookup key=3 → value=%llu\n", (unsigned long long)val);

    /* 遍历所有 key */
    printf("  all keys: ");
    uint32_t cur = 0, next;
    int first = 1;
    while (map_get_next_key(fd, first ? NULL : &cur, &next) == 0) {
        printf("%u ", next);
        cur = next;
        first = 0;
    }
    printf("\n");

    /* 删除 */
    key = 2;
    map_delete(fd, &key);
    printf("  deleted key=2\n");

    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 2. ARRAY Map
 * ────────────────────────────────────────────────────── */
static void demo_array_map(void)
{
    printf("[BPF_MAP_TYPE_ARRAY]\n");

    /* Array key 固定为 uint32_t index，value 可任意 */
    int fd = map_create(BPF_MAP_TYPE_ARRAY, sizeof(uint32_t),
                        sizeof(uint64_t), 8, 0);
    if (fd < 0) { printf("  create failed: %s\n\n", strerror(errno)); return; }

    print_map_info(fd, "array");

    /* 写入 */
    for (uint32_t i = 0; i < 8; i++) {
        uint64_t val = (uint64_t)1 << i;
        map_update(fd, &i, &val, BPF_ANY);
    }

    /* 读取全部 */
    printf("  values: ");
    for (uint32_t i = 0; i < 8; i++) {
        uint64_t val = 0;
        map_lookup(fd, &i, &val);
        printf("%llu ", (unsigned long long)val);
    }
    printf("\n");

    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 3. LRU HASH Map
 * ────────────────────────────────────────────────────── */
static void demo_lru_hash(void)
{
    printf("[BPF_MAP_TYPE_LRU_HASH]\n");

    /* max_entries=4：超出时自动淘汰最久未访问条目 */
    int fd = map_create(BPF_MAP_TYPE_LRU_HASH, sizeof(uint32_t),
                        sizeof(uint64_t), 4, 0);
    if (fd < 0) { printf("  create failed: %s\n\n", strerror(errno)); return; }

    print_map_info(fd, "lru_hash");

    /* 写入 6 个条目（超过 max_entries=4，触发 LRU 淘汰） */
    for (uint32_t i = 1; i <= 6; i++) {
        uint64_t val = i;
        int r = map_update(fd, &i, &val, BPF_ANY);
        printf("  insert key=%u: %s\n", i, r == 0 ? "ok" : strerror(errno));
    }

    /* 验证实际存活条目数（最多 4 个） */
    int count = 0;
    uint32_t cur = 0, next;
    while (map_get_next_key(fd, count == 0 ? NULL : &cur, &next) == 0) {
        cur = next; count++;
    }
    printf("  surviving entries after LRU eviction: %d (max=4)\n", count);

    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 4. QUEUE / STACK
 * ────────────────────────────────────────────────────── */
static void demo_queue_stack(void)
{
    printf("[BPF_MAP_TYPE_QUEUE / STACK]\n");

    /* QUEUE: FIFO，key_size=0 */
    int qfd = map_create(BPF_MAP_TYPE_QUEUE, 0, sizeof(uint32_t), 8, 0);
    if (qfd >= 0) {
        print_map_info(qfd, "queue");
        /* 入队 */
        for (uint32_t v = 10; v <= 50; v += 10)
            map_update(qfd, NULL, &v, BPF_ANY);
        /* 出队（FIFO 顺序） */
        printf("  dequeue order: ");
        uint32_t val;
        union bpf_attr attr = { .map_fd = (uint32_t)qfd,
                                 .value = (uint64_t)(uintptr_t)&val,
                                 .flags = BPF_ANY };
        while (sys_bpf(BPF_MAP_LOOKUP_AND_DELETE_ELEM, &attr, sizeof(attr)) == 0)
            printf("%u ", val);
        printf("\n");
        close(qfd);
    } else {
        printf("  queue create failed: %s\n", strerror(errno));
    }

    /* STACK: LIFO */
    int sfd = map_create(BPF_MAP_TYPE_STACK, 0, sizeof(uint32_t), 8, 0);
    if (sfd >= 0) {
        print_map_info(sfd, "stack");
        for (uint32_t v = 10; v <= 50; v += 10)
            map_update(sfd, NULL, &v, BPF_ANY);
        printf("  pop order (LIFO): ");
        uint32_t val;
        union bpf_attr attr = { .map_fd = (uint32_t)sfd,
                                 .value = (uint64_t)(uintptr_t)&val,
                                 .flags = BPF_ANY };
        while (sys_bpf(BPF_MAP_LOOKUP_AND_DELETE_ELEM, &attr, sizeof(attr)) == 0)
            printf("%u ", val);
        printf("\n");
        close(sfd);
    } else {
        printf("  stack create failed: %s\n", strerror(errno));
    }
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 5. BLOOM FILTER
 * ────────────────────────────────────────────────────── */
static void demo_bloom_filter(void)
{
    printf("[BPF_MAP_TYPE_BLOOM_FILTER]\n");

    /* key_size=0（无 key），value_size=待测元素大小，max_entries=预期元素数 */
    int fd = map_create(BPF_MAP_TYPE_BLOOM_FILTER, 0, sizeof(uint64_t), 1000, 0);
    if (fd < 0) {
        printf("  create failed: %s\n\n", strerror(errno));
        return;
    }
    print_map_info(fd, "bloom_filter");

    /* 插入元素 */
    for (uint64_t v = 0; v < 100; v++)
        map_update(fd, NULL, &v, BPF_ANY);

    /* 查询（LOOKUP 检测成员资格，key=NULL，value=要查的元素） */
    int hits = 0, misses = 0;
    for (uint64_t v = 0; v < 200; v++) {
        union bpf_attr attr = {
            .map_fd = (uint32_t)fd,
            .key    = 0,
            .value  = (uint64_t)(uintptr_t)&v,
        };
        int r = sys_bpf(BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
        if (v < 100)
            hits += (r == 0);
        else
            misses += (r == 0);  /* false positive */
    }
    printf("  inserted 100 elements, queried 200\n");
    printf("  true positives (0-99):   %d/100\n", hits);
    printf("  false positives (100-199): %d/100\n", misses);

    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 6. RINGBUF（内核向用户空间发送数据）
 * ────────────────────────────────────────────────────── */
static void demo_ringbuf(void)
{
    printf("[BPF_MAP_TYPE_RINGBUF]\n");

    /* 大小必须是 2 的幂且是 PAGE_SIZE 的倍数 */
    int fd = map_create(BPF_MAP_TYPE_RINGBUF, 0, 0, 4096, 0);
    if (fd < 0) {
        printf("  create failed: %s\n\n", strerror(errno));
        return;
    }
    print_map_info(fd, "ringbuf");

    /*
     * 获取消费者/生产者指针位置（mmap 到用户空间）
     * 实际生产在 BPF 程序侧（bpf_ringbuf_reserve/submit），
     * 这里只演示 mmap 结构。
     */
    void *cons = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    void *prod = mmap(NULL, getpagesize() * 2, PROT_READ,
                      MAP_SHARED, fd, getpagesize());

    if (cons != MAP_FAILED && prod != MAP_FAILED) {
        printf("  consumer page mapped at %p (read/write)\n", cons);
        printf("  producer page mapped at %p (read-only)\n", prod);
        printf("  consumer_pos=%lu producer_pos=%lu\n",
               *(volatile unsigned long *)cons,
               *(volatile unsigned long *)prod);
        munmap(prod, getpagesize() * 2);
        munmap(cons, getpagesize());
    } else {
        printf("  mmap: %s\n", strerror(errno));
    }

    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 7. 枚举所有已加载的 BPF Map（BPF_MAP_GET_NEXT_ID）
 * ────────────────────────────────────────────────────── */
static const char *map_type_name(uint32_t type)
{
    static const char *names[] = {
        [BPF_MAP_TYPE_HASH]            = "HASH",
        [BPF_MAP_TYPE_ARRAY]           = "ARRAY",
        [BPF_MAP_TYPE_PROG_ARRAY]      = "PROG_ARRAY",
        [BPF_MAP_TYPE_PERF_EVENT_ARRAY]= "PERF_EVENT_ARRAY",
        [BPF_MAP_TYPE_PERCPU_HASH]     = "PERCPU_HASH",
        [BPF_MAP_TYPE_PERCPU_ARRAY]    = "PERCPU_ARRAY",
        [BPF_MAP_TYPE_STACK_TRACE]     = "STACK_TRACE",
        [BPF_MAP_TYPE_LRU_HASH]        = "LRU_HASH",
        [BPF_MAP_TYPE_LRU_PERCPU_HASH] = "LRU_PERCPU_HASH",
        [BPF_MAP_TYPE_LPM_TRIE]        = "LPM_TRIE",
        [BPF_MAP_TYPE_ARRAY_OF_MAPS]   = "ARRAY_OF_MAPS",
        [BPF_MAP_TYPE_HASH_OF_MAPS]    = "HASH_OF_MAPS",
        [BPF_MAP_TYPE_DEVMAP]          = "DEVMAP",
        [BPF_MAP_TYPE_SOCKMAP]         = "SOCKMAP",
        [BPF_MAP_TYPE_CPUMAP]          = "CPUMAP",
        [BPF_MAP_TYPE_XSKMAP]          = "XSKMAP",
        [BPF_MAP_TYPE_SOCKHASH]        = "SOCKHASH",
        [BPF_MAP_TYPE_RINGBUF]         = "RINGBUF",
        [BPF_MAP_TYPE_BLOOM_FILTER]    = "BLOOM_FILTER",
        [BPF_MAP_TYPE_QUEUE]           = "QUEUE",
        [BPF_MAP_TYPE_STACK]           = "STACK",
    };
    if (type < sizeof(names) / sizeof(names[0]) && names[type])
        return names[type];
    return "UNKNOWN";
}

static void demo_enum_maps(void)
{
    printf("[BPF_MAP_GET_NEXT_ID - enumerate system-wide BPF maps]\n");

    union bpf_attr attr = {};
    int count = 0;
    uint32_t id = 0;

    while (1) {
        attr.start_id = id;
        int r = sys_bpf(BPF_MAP_GET_NEXT_ID, &attr, sizeof(attr));
        if (r < 0) break;
        id = attr.next_id;

        /* 获取 FD 和详细信息 */
        union bpf_attr get_attr = { .map_id = id };
        int map_fd = sys_bpf(BPF_MAP_GET_FD_BY_ID, &get_attr, sizeof(get_attr));
        if (map_fd < 0) continue;

        struct bpf_map_info info = {};
        union bpf_attr info_attr = {
            .info.bpf_fd   = (uint32_t)map_fd,
            .info.info_len = sizeof(info),
            .info.info     = (uint64_t)(uintptr_t)&info,
        };
        if (sys_bpf(BPF_OBJ_GET_INFO_BY_FD, &info_attr, sizeof(info_attr)) == 0) {
            printf("  map id=%-5u type=%-20s key=%uB val=%uB entries=%u\n",
                   info.id, map_type_name(info.type),
                   info.key_size, info.value_size, info.max_entries);
            count++;
        }
        close(map_fd);
    }

    if (count == 0)
        printf("  no maps found (or insufficient privilege)\n");
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 8. 枚举已加载的 BPF 程序
 * ────────────────────────────────────────────────────── */
static const char *prog_type_name(uint32_t type)
{
    static const char *names[] = {
        [BPF_PROG_TYPE_SOCKET_FILTER]   = "SOCKET_FILTER",
        [BPF_PROG_TYPE_KPROBE]          = "KPROBE",
        [BPF_PROG_TYPE_SCHED_CLS]       = "SCHED_CLS",
        [BPF_PROG_TYPE_SCHED_ACT]       = "SCHED_ACT",
        [BPF_PROG_TYPE_TRACEPOINT]      = "TRACEPOINT",
        [BPF_PROG_TYPE_XDP]             = "XDP",
        [BPF_PROG_TYPE_PERF_EVENT]      = "PERF_EVENT",
        [BPF_PROG_TYPE_CGROUP_SKB]      = "CGROUP_SKB",
        [BPF_PROG_TYPE_CGROUP_SOCK]     = "CGROUP_SOCK",
        [BPF_PROG_TYPE_SOCK_OPS]        = "SOCK_OPS",
        [BPF_PROG_TYPE_SK_SKB]          = "SK_SKB",
        [BPF_PROG_TYPE_CGROUP_DEVICE]   = "CGROUP_DEVICE",
        [BPF_PROG_TYPE_SK_MSG]          = "SK_MSG",
        [BPF_PROG_TYPE_RAW_TRACEPOINT]  = "RAW_TRACEPOINT",
        [BPF_PROG_TYPE_CGROUP_SOCK_ADDR]= "CGROUP_SOCK_ADDR",
        [BPF_PROG_TYPE_LIRC_MODE2]      = "LIRC_MODE2",
        [BPF_PROG_TYPE_SK_REUSEPORT]    = "SK_REUSEPORT",
        [BPF_PROG_TYPE_FLOW_DISSECTOR]  = "FLOW_DISSECTOR",
        [BPF_PROG_TYPE_CGROUP_SYSCTL]   = "CGROUP_SYSCTL",
        [BPF_PROG_TYPE_TRACING]         = "TRACING",
        [BPF_PROG_TYPE_STRUCT_OPS]      = "STRUCT_OPS",
        [BPF_PROG_TYPE_EXT]             = "EXT",
        [BPF_PROG_TYPE_LSM]             = "LSM",
        [BPF_PROG_TYPE_SK_LOOKUP]       = "SK_LOOKUP",
        [BPF_PROG_TYPE_SYSCALL]         = "SYSCALL",
        [BPF_PROG_TYPE_NETFILTER]       = "NETFILTER",
    };
    if (type < sizeof(names) / sizeof(names[0]) && names[type])
        return names[type];
    return "UNKNOWN";
}

static void demo_enum_progs(void)
{
    printf("[BPF_PROG_GET_NEXT_ID - enumerate system-wide BPF programs]\n");

    union bpf_attr attr = {};
    int count = 0;
    uint32_t id = 0;

    while (1) {
        attr.start_id = id;
        int r = sys_bpf(BPF_PROG_GET_NEXT_ID, &attr, sizeof(attr));
        if (r < 0) break;
        id = attr.next_id;

        union bpf_attr get_attr = { .prog_id = id };
        int prog_fd = sys_bpf(BPF_PROG_GET_FD_BY_ID, &get_attr, sizeof(get_attr));
        if (prog_fd < 0) continue;

        struct bpf_prog_info info = {};
        union bpf_attr info_attr = {
            .info.bpf_fd   = (uint32_t)prog_fd,
            .info.info_len = sizeof(info),
            .info.info     = (uint64_t)(uintptr_t)&info,
        };
        if (sys_bpf(BPF_OBJ_GET_INFO_BY_FD, &info_attr, sizeof(info_attr)) == 0) {
            printf("  prog id=%-5u type=%-22s jited=%d xlated=%uB load_time=%llus\n",
                   info.id, prog_type_name(info.type),
                   info.jited_prog_len > 0,
                   info.xlated_prog_len,
                   (unsigned long long)(info.load_time / 1000000000ULL));
            count++;
        }
        close(prog_fd);
    }

    if (count == 0)
        printf("  no programs found (or insufficient privilege)\n");
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 9. 系统 BPF 配置查询
 * ────────────────────────────────────────────────────── */
static void demo_bpf_sysctl(void)
{
    printf("[BPF sysctl configuration]\n");

    struct {
        const char *path;
        const char *desc;
    } files[] = {
        { "/proc/sys/kernel/unprivileged_bpf_disabled",
          "0=allowed, 1=disabled, 2=non-root limited" },
        { "/proc/sys/net/core/bpf_jit_enable",
          "0=disabled, 1=JIT on, 2=JIT+trace" },
        { "/proc/sys/net/core/bpf_jit_harden",
          "0=off, 1=unprivileged hardening, 2=all" },
        { "/proc/sys/net/core/bpf_jit_kallsyms",
          "1=expose JIT symbols to /proc/kallsyms" },
        { "/proc/sys/kernel/bpf_stats_enabled",
          "1=enable BPF program runtime stats" },
        { NULL, NULL }
    };

    for (int i = 0; files[i].path; i++) {
        FILE *f = fopen(files[i].path, "r");
        if (!f) continue;
        char val[32] = {};
        if (fgets(val, sizeof(val), f)) {
            val[strcspn(val, "\n")] = '\0';
            printf("  %-50s = %-3s  # %s\n", files[i].path, val, files[i].desc);
        }
        fclose(f);
    }
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 10. LPM_TRIE（最长前缀匹配，用于路由表）
 * ────────────────────────────────────────────────────── */
struct lpm_key {
    uint32_t prefixlen;  /* 前缀长度（bits） */
    uint32_t data;       /* IPv4 地址（网络字节序） */
};

static void demo_lpm_trie(void)
{
    printf("[BPF_MAP_TYPE_LPM_TRIE]\n");

    /* value_size 任意，key_size = sizeof(struct bpf_lpm_trie_key) + data */
    int fd = map_create(BPF_MAP_TYPE_LPM_TRIE,
                        sizeof(struct lpm_key), sizeof(uint32_t),
                        256, BPF_F_NO_PREALLOC);
    if (fd < 0) { printf("  create failed: %s\n\n", strerror(errno)); return; }

    print_map_info(fd, "lpm_trie");

    /* 插入路由条目：
     *   0.0.0.0/0     → next_hop=1  (默认路由)
     *   10.0.0.0/8    → next_hop=2
     *   10.0.1.0/24   → next_hop=3
     */
    struct { struct lpm_key k; uint32_t hop; } routes[] = {
        {{ 0,  0x00000000 }, 1},   /* 0.0.0.0/0 */
        {{ 8,  0x0000000a }, 2},   /* 10.0.0.0/8 */
        {{ 24, 0x0001000a }, 3},   /* 10.0.1.0/24 */
    };
    for (int i = 0; i < 3; i++)
        map_update(fd, &routes[i].k, &routes[i].hop, BPF_ANY);

    /* 查询各 IP */
    struct { uint32_t ip; const char *name; } queries[] = {
        { 0x0101000a, "10.0.1.1" },   /* 匹配 /24 */
        { 0x0200000a, "10.0.2.1" },   /* 匹配 /8 */
        { 0x01010101, "1.1.1.1" },    /* 匹配默认路由 */
    };
    for (int i = 0; i < 3; i++) {
        struct lpm_key qk = { .prefixlen = 32, .data = queries[i].ip };
        uint32_t hop = 0;
        if (map_lookup(fd, &qk, &hop) == 0)
            printf("  lookup %-15s → next_hop=%u\n", queries[i].name, hop);
    }

    close(fd);
    printf("\n");
}

int main(void)
{
    printf("=== Linux BPF 子系统 API 演示 (v7.1-rc3 内核特性) ===\n\n");

    demo_bpf_sysctl();
    demo_hash_map();
    demo_array_map();
    demo_lru_hash();
    demo_queue_stack();
    demo_bloom_filter();
    demo_ringbuf();
    demo_lpm_trie();
    demo_enum_maps();
    demo_enum_progs();

    return 0;
}
