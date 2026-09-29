#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 4096

/* 用系统调用实现一个简化版 cp：源文件 -> 目标文件 */
int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "用法: %s <源文件> <目标文件>\n", argv[0]);
        return 1;
    }

    int in = open(argv[1], O_RDONLY);
    if (in < 0) {
        perror("open 源文件");
        return 1;
    }

    /* O_CREAT: 不存在就创建；O_TRUNC: 存在就清空；0644 是权限 */
    int out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        perror("open 目标文件");
        close(in);
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t n;
    long total = 0;

    while ((n = read(in, buf, sizeof buf)) > 0) {
        ssize_t off = 0;
        while (off < n) {
            ssize_t w = write(out, buf + off, n - off);
            if (w < 0) { perror("write"); close(in); close(out); return 1; }
            off += w;
        }
        total += n;
    }
    if (n < 0)
        perror("read");

    close(in);
    close(out);
    printf("已复制 %ld 字节\n", total);
    return 0;
}
