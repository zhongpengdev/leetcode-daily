#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define BUFSIZE (64 * 1024) // 64KB
#define RUNS 3

// 计时工具
typedef struct {
    struct timespec start;
    struct timespec end;
} Timer;

static inline void timer_start(Timer *t) { clock_gettime(CLOCK_MONOTONIC, &t->start); }

static inline double timer_end(Timer *t) {
    clock_gettime(CLOCK_MONOTONIC, &t->end);
    return (t->end.tv_sec - t->start.tv_sec) + (t->end.tv_nsec - t->start.tv_nsec) / 1e9;
}

// 1. 传统 IO: read + write
double bench_traditional(const char *src, const char *dst) {
    int fd_in = open(src, O_RDONLY);
    int fd_out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_in < 0 || fd_out < 0) { perror("open"); return -1; }

    char *buf = malloc(BUFSIZE);
    Timer t;
    timer_start(&t);

    ssize_t n;
    while ((n = read(fd_in, buf, BUFSIZE)) > 0) {
        write(fd_out, buf, n);
    }

    double elapsed = timer_end(&t);

    free(buf);
    close(fd_in);
    close(fd_out);
    return elapsed;
}

// 2. mmap + write
double bench_mmap(const char *src, const char *dst) {
    int fd_in = open(src, O_RDONLY);
    int fd_out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_in < 0 || fd_out < 0) { perror("open"); return -1; }

    struct stat st;
    fstat(fd_in, &st);
    off_t filesize = st.st_size;

    Timer t;
    timer_start(&t);

    void *addr = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd_in, 0);
    if (addr == MAP_FAILED) { perror("mmap"); return -1; }

    // 分块写，避免一次 write 过大
    off_t written = 0;
    while (written < filesize) {
        size_t chunk = filesize - written;
        if (chunk > BUFSIZE) chunk = BUFSIZE;
        write(fd_out, (char *)addr + written, chunk);
        written += chunk;
    }

    double elapsed = timer_end(&t);

    munmap(addr, filesize);
    close(fd_in);
    close(fd_out);
    return elapsed;
}

// 3. sendfile
double bench_sendfile(const char *src, const char *dst) {
    int fd_in = open(src, O_RDONLY);
    int fd_out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_in < 0 || fd_out < 0) { perror("open"); return -1; }

    struct stat st;
    fstat(fd_in, &st);
    off_t filesize = st.st_size;

    Timer t;
    timer_start(&t);

    off_t offset = 0;
    while (offset < filesize) {
        ssize_t n = sendfile(fd_out, fd_in, &offset, filesize - offset);
        if (n <= 0) break;
    }

    double elapsed = timer_end(&t);

    close(fd_in);
    close(fd_out);
    return elapsed;
}

// 4. splice
double bench_splice(const char *src, const char *dst) {
    int fd_in = open(src, O_RDONLY);
    int fd_out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_in < 0 || fd_out < 0) { perror("open"); return -1; }

    int pipefd[2];
    if (pipe(pipefd) < 0) { perror("pipe"); return -1; }

    Timer t;
    timer_start(&t);

    ssize_t n;
    while ((n = splice(fd_in, NULL, pipefd[1], NULL, BUFSIZE, SPLICE_F_MOVE)) > 0) {
        splice(pipefd[0], NULL, fd_out, NULL, n, SPLICE_F_MOVE);
    }

    double elapsed = timer_end(&t);

    close(pipefd[0]);
    close(pipefd[1]);
    close(fd_in);
    close(fd_out);
    return elapsed;
}

// 输出格式化
void print_size(off_t size) {
    if (size >= 1024 * 1024 * 1024)
        printf("%.2f GB", size / (1024.0 * 1024 * 1024));
    else if (size >= 1024 * 1024)
        printf("%.2f MB", size / (1024.0 * 1024));
    else
        printf("%.2f KB", size / 1024.0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <testfile>\n", argv[0]);
        return 1;
    }

    const char *src = argv[1];
    const char *dst = "/tmp/_zc_bench_out";

    struct stat st;
    if (stat(src, &st) < 0) { perror("stat"); return 1; }

    printf("=== 零拷贝基准测试 ===\n");
    printf("文件大小: ");
    print_size(st.st_size);
    printf("\n缓冲区: %d KB, 每轮 %d 次取平均\n\n", BUFSIZE / 1024, RUNS);

    printf("%-14s %10s\n", "方式", "耗时(s)");
    printf("-----------------------------\n");

    // 传统 IO
    double total = 0;
    for (int i = 0; i < RUNS; i++) total += bench_traditional(src, dst);
    printf("%-14s %10.4f\n", "read+write", total / RUNS);

    // mmap
    total = 0;
    for (int i = 0; i < RUNS; i++) total += bench_mmap(src, dst);
    printf("%-14s %10.4f\n", "mmap", total / RUNS);

    // sendfile
    total = 0;
    for (int i = 0; i < RUNS; i++) total += bench_sendfile(src, dst);
    printf("%-14s %10.4f\n", "sendfile", total / RUNS);

    // splice
    total = 0;
    for (int i = 0; i < RUNS; i++) total += bench_splice(src, dst);
    printf("%-14s %10.4f\n", "splice", total / RUNS);

    // 清理临时文件
    unlink(dst);

    return 0;
}
