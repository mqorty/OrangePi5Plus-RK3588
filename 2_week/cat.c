#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 4096

/* 用系统调用实现一个简化版 cat：把文件内容写到标准输出 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "用法: %s <文件>\n", argv[0]);
        return 1;
    }

    /* open 返回一个文件描述符(fd)，失败返回 -1 并设置 errno */
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");          /* perror 会打印 "open: <errno 描述>" */
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t n;

    /* read 返回实际读到的字节数；0 表示到文件尾；-1 表示出错 */
    while ((n = read(fd, buf, sizeof buf)) > 0) {
        ssize_t off = 0;
        /* write 可能只写一部分，必须循环直到写完 */
        while (off < n) {
            ssize_t w = write(STDOUT_FILENO, buf + off, n - off);
            if (w < 0) {
                perror("write");
                close(fd);
                return 1;
            }
            off += w;
        }
    }
    if (n < 0)
        perror("read");

    close(fd);                   /* 用完必须关闭，避免 fd 泄漏 */
    return 0;
}
