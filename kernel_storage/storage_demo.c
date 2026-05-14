/*
 * Linux 块设备/存储 API 演示程序
 *
 * 演示内容：
 *   1. 查询块设备队列参数（通过 sysfs）
 *   2. 直接 IO（O_DIRECT）绕过 page cache
 *   3. io_uring 异步 IO（使用 syscall 接口）
 *   4. fallocate / punch hole / zero range 文件空洞操作
 *   5. fsync / fdatasync / sync_file_range
 *   6. ioctl BLKGETSIZE64 / BLKBSZGET 块设备信息
 *   7. /proc/diskstats 解析 IO 统计
 *   8. posix_fadvise 预读提示
 *
 * 编译：gcc -o storage_demo storage_demo.c -Wall -O2
 * 运行：./storage_demo [block-device]  (默认读 /sys/block/sda，可指定其他设备)
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/uio.h>
#include <linux/fs.h>
#include <linux/io_uring.h>

#define PAGE_SIZE   4096
#define SECTOR_SIZE 512

/* ──────────────────────────────────────────────────────
 * 1. 读取块设备 sysfs 队列参数
 * ────────────────────────────────────────────────────── */
static void read_sysfs_attr(const char *dev, const char *attr)
{
    char path[256];
    snprintf(path, sizeof(path), "/sys/block/%s/queue/%s", dev, attr);

    FILE *f = fopen(path, "r");
    if (!f) return;

    char val[128] = {};
    if (fgets(val, sizeof(val), f)) {
        /* 去除尾部换行 */
        val[strcspn(val, "\n")] = '\0';
        printf("  %-30s = %s\n", attr, val);
    }
    fclose(f);
}

static void demo_sysfs_queue(const char *devname)
{
    printf("[sysfs queue parameters: /sys/block/%s/queue/]\n", devname);

    const char *attrs[] = {
        "scheduler",
        "nr_requests",
        "max_sectors_kb",
        "max_segments",
        "max_segment_size",
        "logical_block_size",
        "physical_block_size",
        "optimal_io_size",
        "rotational",          /* 0=SSD, 1=HDD */
        "write_cache",
        "discard_granularity",
        "discard_max_bytes",
        "write_zeroes_max_bytes",
        "io_poll",             /* 轮询模式 */
        "wbt_lat_usec",        /* WBT 延迟目标 */
        NULL
    };

    for (int i = 0; attrs[i]; i++)
        read_sysfs_attr(devname, attrs[i]);

    printf("\n");
}

static const char *find_first_block_dev(void)
{
    static char name[64];
    DIR *d = opendir("/sys/block");
    if (!d) return "sda";

    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_name[0] == '.') continue;
        /* 跳过 loop/ram 设备 */
        if (strncmp(e->d_name, "loop", 4) == 0) continue;
        if (strncmp(e->d_name, "ram",  3) == 0) continue;
        strncpy(name, e->d_name, sizeof(name) - 1);
        closedir(d);
        return name;
    }
    closedir(d);
    return "sda";
}

/* ──────────────────────────────────────────────────────
 * 2. 直接 IO（O_DIRECT）
 * ────────────────────────────────────────────────────── */
static void demo_direct_io(void)
{
    printf("[O_DIRECT - bypass page cache]\n");

    const char *tmpfile = "/tmp/storage_demo_direct.bin";
    int fd = open(tmpfile, O_RDWR | O_CREAT | O_DIRECT | O_TRUNC, 0600);
    if (fd < 0) {
        printf("  O_DIRECT open failed: %s\n\n", strerror(errno));
        return;
    }

    /* O_DIRECT 要求缓冲区和偏移量按 512 字节对齐 */
    void *buf = NULL;
    if (posix_memalign(&buf, 512, PAGE_SIZE) != 0) {
        perror("posix_memalign"); close(fd); return;
    }

    memset(buf, 0x5A, PAGE_SIZE);

    ssize_t n = write(fd, buf, PAGE_SIZE);
    if (n == PAGE_SIZE)
        printf("  wrote %zd bytes with O_DIRECT (DMA直写磁盘，不经 page cache)\n", n);
    else
        printf("  write failed: %s\n", strerror(errno));

    /* 读回验证 */
    lseek(fd, 0, SEEK_SET);
    memset(buf, 0, PAGE_SIZE);
    n = read(fd, buf, PAGE_SIZE);
    if (n == PAGE_SIZE && ((uint8_t *)buf)[0] == 0x5A)
        printf("  read back OK, data matches\n");

    free(buf);
    close(fd);
    unlink(tmpfile);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 3. io_uring 异步 IO（通过系统调用直接使用）
 * ────────────────────────────────────────────────────── */

/* io_uring 的最小封装（不依赖 liburing） */
static int io_uring_setup(unsigned entries, struct io_uring_params *p)
{
    return (int)syscall(SYS_io_uring_setup, entries, p);
}

static int io_uring_enter(int fd, unsigned to_submit, unsigned min_complete,
                           unsigned flags)
{
    return (int)syscall(SYS_io_uring_enter, fd, to_submit, min_complete,
                        flags, NULL, 0);
}

static void demo_io_uring(void)
{
    printf("[io_uring - async IO]\n");

    struct io_uring_params params = {};
    int ring_fd = io_uring_setup(4, &params);
    if (ring_fd < 0) {
        printf("  io_uring_setup failed: %s\n\n", strerror(errno));
        return;
    }

    printf("  io_uring created, ring_fd=%d\n", ring_fd);
    printf("  features: 0x%08x\n", params.features);

    /* 打印支持的特性 */
    if (params.features & IORING_FEAT_SINGLE_MMAP)
        printf("    IORING_FEAT_SINGLE_MMAP: sq/cq 共享单一 mmap\n");
    if (params.features & IORING_FEAT_NODROP)
        printf("    IORING_FEAT_NODROP: CQ 溢出时阻塞而非丢弃\n");
    if (params.features & IORING_FEAT_SUBMIT_STABLE)
        printf("    IORING_FEAT_SUBMIT_STABLE: 提交后缓冲区可立即复用\n");
    if (params.features & IORING_FEAT_FAST_POLL)
        printf("    IORING_FEAT_FAST_POLL: 内核态快速轮询路径\n");
    if (params.features & IORING_FEAT_SQPOLL_NONFIXED)
        printf("    IORING_FEAT_SQPOLL_NONFIXED: SQPOLL 支持非固定文件\n");

    printf("  sq_entries=%u cq_entries=%u\n",
           params.sq_entries, params.cq_entries);

    close(ring_fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 4. fallocate —— 文件空间预分配/打洞/归零
 * ────────────────────────────────────────────────────── */
static void demo_fallocate(void)
{
    printf("[fallocate / punch hole / zero range]\n");

    const char *tmpfile = "/tmp/storage_demo_fallocate.bin";
    int fd = open(tmpfile, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) { perror("open"); return; }

    /* 预分配 1MB，不改变文件大小（FALLOC_FL_KEEP_SIZE） */
    if (fallocate(fd, FALLOC_FL_KEEP_SIZE, 0, 1 * 1024 * 1024) == 0)
        printf("  FALLOC_FL_KEEP_SIZE: 1MB pre-allocated (no fragmentation)\n");
    else
        printf("  FALLOC_FL_KEEP_SIZE: %s\n", strerror(errno));

    /* 扩展文件到 2MB 并填充 */
    ftruncate(fd, 2 * 1024 * 1024);
    char buf[4096];
    memset(buf, 0xFF, sizeof(buf));
    pwrite(fd, buf, sizeof(buf), 0);
    pwrite(fd, buf, sizeof(buf), 1024 * 1024);

    /* 打洞（punch hole）：释放 512KB~1MB 的物理块，文件大小不变 */
    int ret = fallocate(fd, FALLOC_FL_PUNCH_HOLE | FALLOC_FL_KEEP_SIZE,
                        512 * 1024, 512 * 1024);
    if (ret == 0)
        printf("  PUNCH_HOLE: 512KB~1MB physical blocks released (sparse file)\n");
    else
        printf("  PUNCH_HOLE: %s\n", strerror(errno));

    /* 零范围（zero range）：将数据清零但保留物理块 */
    ret = fallocate(fd, FALLOC_FL_ZERO_RANGE, 0, 4096);
    if (ret == 0)
        printf("  ZERO_RANGE: first 4KB zeroed (blocks retained)\n");
    else
        printf("  ZERO_RANGE: %s\n", strerror(errno));

    struct stat st;
    fstat(fd, &st);
    printf("  file size=%lld bytes, blocks=%lld (512B units)\n",
           (long long)st.st_size, (long long)st.st_blocks);

    close(fd);
    unlink(tmpfile);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 5. fsync / fdatasync / sync_file_range
 * ────────────────────────────────────────────────────── */
static void demo_fsync(void)
{
    printf("[fsync / fdatasync / sync_file_range]\n");

    const char *tmpfile = "/tmp/storage_demo_sync.bin";
    int fd = open(tmpfile, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) { perror("open"); return; }

    char buf[4096];
    memset(buf, 0x77, sizeof(buf));
    write(fd, buf, sizeof(buf));

    /* fdatasync：只刷数据，不刷元数据（mtime 等），比 fsync 快 */
    if (fdatasync(fd) == 0)
        printf("  fdatasync: data flushed to device (skips metadata)\n");

    write(fd, buf, sizeof(buf));

    /* sync_file_range：异步刷指定范围，不等待完成（SYNC_FILE_RANGE_WRITE） */
    if (sync_file_range(fd, 0, 4096, SYNC_FILE_RANGE_WRITE) == 0)
        printf("  sync_file_range WRITE: async writeback started for 4KB\n");

    /* 等待指定范围写入完成 */
    if (sync_file_range(fd, 0, 4096, SYNC_FILE_RANGE_WAIT_AFTER) == 0)
        printf("  sync_file_range WAIT_AFTER: write completed\n");

    /* fsync：刷数据 + 元数据（最安全，最慢） */
    if (fsync(fd) == 0)
        printf("  fsync: data + metadata flushed\n");

    close(fd);
    unlink(tmpfile);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 6. 块设备 ioctl 信息查询
 * ────────────────────────────────────────────────────── */
static void demo_blk_ioctl(const char *devpath)
{
    printf("[block device ioctl: %s]\n", devpath);

    int fd = open(devpath, O_RDONLY);
    if (fd < 0) {
        printf("  open failed: %s\n\n", strerror(errno));
        return;
    }

    uint64_t size64 = 0;
    if (ioctl(fd, BLKGETSIZE64, &size64) == 0)
        printf("  BLKGETSIZE64: device size = %llu bytes (%.1f GB)\n",
               (unsigned long long)size64,
               (double)size64 / (1024.0 * 1024 * 1024));

    int bsz = 0;
    if (ioctl(fd, BLKBSZGET, &bsz) == 0)
        printf("  BLKBSZGET:   block size = %d bytes\n", bsz);

    int ssz = 0;
    if (ioctl(fd, BLKSSZGET, &ssz) == 0)
        printf("  BLKSSZGET:   sector size = %d bytes\n", ssz);

    unsigned int pbsz = 0;
    if (ioctl(fd, BLKPBSZGET, &pbsz) == 0)
        printf("  BLKPBSZGET:  physical block size = %u bytes\n", pbsz);

    close(fd);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 7. 解析 /proc/diskstats
 * ────────────────────────────────────────────────────── */
static void demo_diskstats(const char *devname)
{
    printf("[/proc/diskstats: %s]\n", devname);

    FILE *f = fopen("/proc/diskstats", "r");
    if (!f) { perror("open diskstats"); return; }

    /* 字段说明（内核文档 Documentation/admin-guide/iostats.rst）：
     * major minor name
     *   [1] reads_completed  [2] reads_merged  [3] sectors_read  [4] ms_reading
     *   [5] writes_completed [6] writes_merged [7] sectors_written [8] ms_writing
     *   [9] io_in_progress  [10] ms_doing_io  [11] weighted_ms_io
     *  [12] discards_completed ... [16] flush_completed [17] ms_flushing
     */
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char name[64];
        unsigned int maj, min;
        unsigned long long rc, rm, rs, rms, wc, wm, ws, wms, io_prog, iotms, wiotms;
        unsigned long long dc, dm, ds, dms, fc, fms;

        int n = sscanf(line,
            "%u %u %63s "
            "%llu %llu %llu %llu "
            "%llu %llu %llu %llu "
            "%llu %llu %llu "
            "%llu %llu %llu %llu "
            "%llu %llu",
            &maj, &min, name,
            &rc, &rm, &rs, &rms,
            &wc, &wm, &ws, &wms,
            &io_prog, &iotms, &wiotms,
            &dc, &dm, &ds, &dms,
            &fc, &fms);

        if (n < 14) continue;
        if (strcmp(name, devname) != 0) continue;

        printf("  reads:    %llu completed, %llu sectors (%.1f MB), %llu ms\n",
               rc, rs, (double)rs * 512 / (1024 * 1024), rms);
        printf("  writes:   %llu completed, %llu sectors (%.1f MB), %llu ms\n",
               wc, ws, (double)ws * 512 / (1024 * 1024), wms);
        printf("  io_prog:  %llu\n", io_prog);
        printf("  io_ticks: %llu ms (time device was busy)\n", iotms);
        if (n >= 18)
            printf("  discards: %llu completed, %llu sectors\n", dc, ds);
        if (n >= 20)
            printf("  flushes:  %llu completed, %llu ms\n", fc, fms);
        break;
    }
    fclose(f);
    printf("\n");
}

/* ──────────────────────────────────────────────────────
 * 8. posix_fadvise 预读提示
 * ────────────────────────────────────────────────────── */
static void demo_fadvise(void)
{
    printf("[posix_fadvise]\n");

    const char *tmpfile = "/tmp/storage_demo_fadvise.bin";
    int fd = open(tmpfile, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) { perror("open"); return; }

    /* 写入 1MB 数据 */
    char buf[4096];
    memset(buf, 0x33, sizeof(buf));
    for (int i = 0; i < 256; i++)
        write(fd, buf, sizeof(buf));

    /* POSIX_FADV_SEQUENTIAL：内核应积极预读（适合流式处理） */
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL) == 0)
        printf("  POSIX_FADV_SEQUENTIAL: kernel will prefetch aggressively\n");

    /* POSIX_FADV_WILLNEED：立即触发预读（异步） */
    if (posix_fadvise(fd, 0, 512 * 1024, POSIX_FADV_WILLNEED) == 0)
        printf("  POSIX_FADV_WILLNEED: async readahead triggered for 512KB\n");

    /* POSIX_FADV_DONTNEED：告知内核丢弃 page cache（适合一次性读完的文件） */
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_DONTNEED) == 0)
        printf("  POSIX_FADV_DONTNEED: page cache pages released\n");

    /* POSIX_FADV_RANDOM：禁用预读（随机访问场景） */
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_RANDOM) == 0)
        printf("  POSIX_FADV_RANDOM: readahead disabled (random access pattern)\n");

    close(fd);
    unlink(tmpfile);
    printf("\n");
}

int main(int argc, char *argv[])
{
    const char *devname = (argc > 1) ? argv[1] : find_first_block_dev();

    printf("=== Linux 块设备/存储 API 演示 (v7.1-rc3 内核特性) ===\n");
    printf("Using block device: %s\n\n", devname);

    /* sysfs 参数查询 */
    demo_sysfs_queue(devname);

    /* IO 操作演示 */
    demo_direct_io();
    demo_io_uring();
    demo_fallocate();
    demo_fsync();
    demo_fadvise();

    /* 块设备 ioctl（构造路径尝试打开） */
    char devpath[64];
    snprintf(devpath, sizeof(devpath), "/dev/%s", devname);
    demo_blk_ioctl(devpath);

    /* IO 统计 */
    demo_diskstats(devname);

    return 0;
}
