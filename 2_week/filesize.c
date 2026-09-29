#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "用法: %s <文件1> [文件2 ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            perror(argv[i]);
            continue;
        }

        // lseek 移到末尾，返回值 = 文件大小
        off_t size = lseek(fd, 0, SEEK_END);
        if (size == (off_t)-1) {
            perror("lseek");
            close(fd);
            continue;
        }

        printf("%s: %lld bytes\n", argv[i], (long long)size);

        close(fd);
    }

    return 0;
}
