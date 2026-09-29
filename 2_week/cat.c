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
        fprintf(stderr, "用法: %s <文件1> [文件2 文件3 ...]\n", argv[0]);
        return 1;
    }

    // 循环处理每一个输入文件，从 argv[1] 开始
    for (int i = 1; i < argc; i++)
    {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror(argv[i]);   // 打印哪个文件出错
            continue;          // 跳过当前文件，继续下一个
        }

        char buf[BUF_SIZE];
        ssize_t n;
        while ((n = read(fd, buf, sizeof(buf))) > 0) {
            ssize_t off = 0;
            // write 可能部分写入，循环写完缓冲区全部数据
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

        if (n < 0) {
            perror(argv[i]);
        }

        close(fd); // 关闭当前文件，处理下一个
    }
   return 0;
}
