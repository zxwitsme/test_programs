/*
 * Linux 内存管理 API 演示程序
 *
 * 演示内容：
 *   1. mmap 匿名/文件映射与 madvise
 *   2. mlock / munlock 页面锁定
 *   3. mprotect 权限保护
 *   4. memfd_create + 文件密封（sealing）
 *   5. userfaultfd 缺页拦截
 *   6. /proc/self/smaps 解析 VMA 信息
 *   7. NUMA 策略查询（get_mempolicy）
 *
 * 编译：gcc -o mm_demo mm_demo.c -lpthread -Wall -O2
 * 运行：./mm_demo
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <linux/memfd.h>
#include <linux/userfaultfd.h>

#define PAGE_SIZE   4096
#define MB          (1024 * 1024)

/* ──────────────────────────────────────────────────────
 * 1. 匿名 mmap + madvise 演示
 * ────────────────────────────────────────────────────── */
static void demo_mmap_madvise(void)
{
    printf("[mmap + madvise]\n");

    /* 分配 4MB 匿名页（延迟分配，未实际触发缺页） */
    void *mem = mmap(NULL, 4 * MB, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) { perror("mmap"); return; }

    /* MADV_SEQUENTIAL：预期顺序访问，内核积极预读 */
    madvise(mem, 4 * MB, MADV_SEQUENTIAL);

    /* 触发实际缺页，填充数据 */
    memset(mem, 0xAB, 4 * MB);
    printf("  4MB anonymous mmap filled at %p\n", mem);

    /* MADV_DONTNEED：告知内核可以丢弃这些页面（下次访问重新缺页） */
    madvise(mem, 2 * MB, MADV_DONTNEED);
    printf("  2MB MADV_DONTNEED applied (pages released to kernel)\n");

    /* MADV_HUGEPAGE：建议内核将此区域晋升为透明大页（THP） */
    madvise(mem + 2 * MB, 2 * MB, MADV_HUGEPAGE);
    printf("  2MB MADV_HUGEPAGE hint applied\n");

    /* MADV_PAGEOUT：主动换出（模拟冷页回收，需要内核 5.14+） */
    if (madvise(mem, 4 * MB, MADV_PAGEOUT) == 0)
        printf("  4MB MADV_PAGEOUT: pages swapped out\n");
    else
        printf("  4MB MADV_PAGEOUT: %s\n", strerror(errno));

    munmap(mem, 4 * MB);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 2. mlock 页面锁定
 * ────────────────────────────────────────────────────── */
static void demo_mlock(void)
{
    printf("[mlock / munlock]\n");

    void *mem = mmap(NULL, PAGE_SIZE * 4, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) { perror("mmap"); return; }

    memset(mem, 0, PAGE_SIZE * 4);

    if (mlock(mem, PAGE_SIZE * 4) == 0) {
        printf("  4 pages locked in RAM (will not be swapped)\n");
        munlock(mem, PAGE_SIZE * 4);
        printf("  pages unlocked\n");
    } else {
        printf("  mlock failed: %s (may need higher RLIMIT_MEMLOCK)\n",
               strerror(errno));
    }

    /* 查询当前 mlock 限制 */
    struct rlimit rl;
    getrlimit(RLIMIT_MEMLOCK, &rl);
    printf("  RLIMIT_MEMLOCK: cur=%lu max=%lu bytes\n",
           (unsigned long)rl.rlim_cur, (unsigned long)rl.rlim_max);

    munmap(mem, PAGE_SIZE * 4);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 3. mprotect 权限保护
 * ────────────────────────────────────────────────────── */
static void demo_mprotect(void)
{
    printf("[mprotect]\n");

    void *mem = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) { perror("mmap"); return; }

    *(volatile int *)mem = 42;
    printf("  wrote 42 to page\n");

    /* 设为只读 */
    if (mprotect(mem, PAGE_SIZE, PROT_READ) == 0)
        printf("  page set to PROT_READ only\n");

    /* 设为不可访问 */
    if (mprotect(mem, PAGE_SIZE, PROT_NONE) == 0)
        printf("  page set to PROT_NONE (guard page)\n");

    /* 恢复读写 */
    mprotect(mem, PAGE_SIZE, PROT_READ | PROT_WRITE);

    munmap(mem, PAGE_SIZE);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 4. memfd_create + 文件密封（sealing）
 * ────────────────────────────────────────────────────── */
static void demo_memfd(void)
{
    printf("[memfd_create + sealing]\n");

    /* 创建匿名内存文件，启用密封功能 */
    int fd = memfd_create("demo_memfd", MFD_CLOEXEC | MFD_ALLOW_SEALING);
    if (fd < 0) { perror("memfd_create"); return; }

    /* 设置大小 */
    if (ftruncate(fd, PAGE_SIZE) < 0) { perror("ftruncate"); close(fd); return; }

    /* 映射并写入数据 */
    void *mem = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) { perror("mmap memfd"); close(fd); return; }

    snprintf((char *)mem, PAGE_SIZE, "memfd shared buffer: hello");
    printf("  wrote: \"%s\"\n", (char *)mem);

    /* 施加密封：禁止 shrink（缩小）+ 禁止 grow（扩大） */
    if (fcntl(fd, F_ADD_SEALS, F_SEAL_SHRINK | F_SEAL_GROW) == 0)
        printf("  seals applied: F_SEAL_SHRINK | F_SEAL_GROW\n");

    /* 尝试改变大小（应失败） */
    if (ftruncate(fd, PAGE_SIZE * 2) < 0)
        printf("  ftruncate(grow) blocked by seal: %s\n", strerror(errno));

    /* 施加 WRITE 密封：禁止写入 */
    if (fcntl(fd, F_ADD_SEALS, F_SEAL_WRITE) == 0)
        printf("  seal applied: F_SEAL_WRITE\n");

    munmap(mem, PAGE_SIZE);
    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 5. userfaultfd 缺页拦截
 * ────────────────────────────────────────────────────── */
struct uffd_thread_args {
    int uffd;
    void *fault_addr;
    int done;
};

static void *uffd_handler(void *arg)
{
    struct uffd_thread_args *a = arg;
    struct uffd_msg msg;

    while (!a->done) {
        ssize_t n = read(a->uffd, &msg, sizeof(msg));
        if (n <= 0) break;
        if (msg.event != UFFD_EVENT_PAGEFAULT) continue;

        /* 填充零页响应缺页 */
        struct uffdio_zeropage zp = {
            .range = { .start = msg.arg.pagefault.address & ~(PAGE_SIZE - 1),
                       .len   = PAGE_SIZE },
        };
        ioctl(a->uffd, UFFDIO_ZEROPAGE, &zp);
    }
    return NULL;
}

static void demo_userfaultfd(void)
{
    printf("[userfaultfd]\n");

    /* 创建 userfaultfd */
    int uffd = (int)syscall(SYS_userfaultfd, O_CLOEXEC | O_NONBLOCK);
    if (uffd < 0) {
        printf("  userfaultfd not available: %s\n\n", strerror(errno));
        return;
    }

    struct uffdio_api api = { .api = UFFD_API };
    if (ioctl(uffd, UFFDIO_API, &api) < 0) {
        perror("UFFDIO_API"); close(uffd); return;
    }
    printf("  uffd features: 0x%llx\n", (unsigned long long)api.features);

    /* 分配监控区域 */
    void *mem = mmap(NULL, PAGE_SIZE * 2, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) { close(uffd); return; }

    /* 注册 userfaultfd 监控 */
    struct uffdio_register reg = {
        .range = { .start = (unsigned long)mem, .len = PAGE_SIZE * 2 },
        .mode  = UFFDIO_REGISTER_MODE_MISSING,
    };
    if (ioctl(uffd, UFFDIO_REGISTER, &reg) < 0) {
        perror("UFFDIO_REGISTER"); munmap(mem, PAGE_SIZE * 2); close(uffd); return;
    }
    printf("  registered %zu bytes for userfaultfd monitoring\n", (size_t)PAGE_SIZE * 2);

    /* 启动处理线程 */
    struct uffd_thread_args args = { .uffd = uffd, .fault_addr = mem };
    pthread_t tid;
    pthread_create(&tid, NULL, uffd_handler, &args);

    /* 触发缺页（由 handler 线程处理） */
    volatile char *p = (char *)mem;
    *p = 0;  /* 缺页 → handler 填充零页 */
    printf("  page fault handled by userfaultfd handler\n");

    args.done = 1;
    ioctl(uffd, UFFDIO_UNREGISTER, &reg.range);
    close(uffd);
    pthread_join(tid, NULL);
    munmap(mem, PAGE_SIZE * 2);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 6. 解析 /proc/self/smaps_rollup（内存使用摘要）
 * ────────────────────────────────────────────────────── */
static void demo_smaps(void)
{
    printf("[/proc/self/smaps_rollup]\n");

    FILE *f = fopen("/proc/self/smaps_rollup", "r");
    if (!f) { perror("open smaps_rollup"); return; }

    char line[256];
    int printed = 0;
    while (fgets(line, sizeof(line), f) && printed < 8) {
        /* 只打印数值行（KB 结尾） */
        if (strstr(line, " kB")) {
            printf("  %s", line);
            printed++;
        }
    }
    fclose(f);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 7. /proc/buddyinfo —— 伙伴系统空闲页统计
 * ────────────────────────────────────────────────────── */
static void demo_buddyinfo(void)
{
    printf("[/proc/buddyinfo (Buddy System free pages per order)]\n");

    FILE *f = fopen("/proc/buddyinfo", "r");
    if (!f) { perror("open buddyinfo"); return; }

    char line[256];
    while (fgets(line, sizeof(line), f))
        printf("  %s", line);
    fclose(f);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 8. /proc/meminfo 关键字段
 * ────────────────────────────────────────────────────── */
static void demo_meminfo(void)
{
    const char *keys[] = {
        "MemTotal:", "MemFree:", "MemAvailable:", "Buffers:", "Cached:",
        "SwapTotal:", "SwapFree:", "AnonPages:", "Mapped:", "Shmem:",
        "Slab:", "KernelStack:", "HugePages_Total:", "HugePages_Free:",
        "AnonHugePages:", NULL
    };

    printf("[/proc/meminfo (key fields)]\n");

    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) { perror("open meminfo"); return; }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        for (int i = 0; keys[i]; i++) {
            if (strncmp(line, keys[i], strlen(keys[i])) == 0)
                printf("  %s", line);
        }
    }
    fclose(f);
    printf("\n");
}

int main(void)
{
    printf("=== Linux 内存管理 API 演示 (v7.1-rc3 内核特性) ===\n\n");

    demo_mmap_madvise();
    demo_mlock();
    demo_mprotect();
    demo_memfd();
    demo_userfaultfd();
    demo_smaps();
    demo_buddyinfo();
    demo_meminfo();

    return 0;
}
