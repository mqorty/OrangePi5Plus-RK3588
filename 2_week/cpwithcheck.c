#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "用法: %s <源文件> <目标文件>\n", argv[0]);
        return 1;
    }

    const char *srcpath = argv[1];
    const char *dstpath = argv[2];

    // 打开源文件：只读
    int srcfd = open(srcpath, O_RDONLY);
    if (srcfd < 0) {
        perror(srcpath);
        return 1;
    }

    // O_CREAT|O_EXCL：文件不存在才创建；已存在直接失败 EEXIST
    int dstfd = open(dstpath, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (dstfd < 0) {
        perror(dstpath);
        close(srcfd);
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t nread;
    while ((nread = read(srcfd, buf, sizeof(buf))) > 0) {
        ssize_t off = 0;
        // write 可能部分写入，循环写完缓冲区
        while (off < nread) {
            ssize_t w = write(dstfd, buf + off, nread - off);
            if (w < 0) {
                perror("write");
                close(srcfd);
                close(dstfd);
                return 1;
            }
            off += w;
        }
    }

    if (nread < 0) {
        perror("read");
        close(srcfd);
        close(dstfd);
        return 1;
    }

    close(srcfd);
    close(dstfd);
    return 0;
}
