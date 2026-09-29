#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int main(void)
{
    printf("== 访问模式（三选一，本质是 0/1/2）==\n");
    printf("O_RDONLY = %d\nO_WRONLY = %d\nO_RDWR   = %d\n\n",
           O_RDONLY, O_WRONLY, O_RDWR);

    printf("== 其他 flag（各占二进制一位，可 | 叠加）==\n");
    printf("O_CREAT  = 0%-4o = %4d\n", O_CREAT,  O_CREAT);
    printf("O_EXCL   = 0%-4o = %4d\n", O_EXCL,   O_EXCL);
    printf("O_TRUNC  = 0%-4o = %4d\n", O_TRUNC,  O_TRUNC);
    printf("O_APPEND = 0%-4o = %4d\n\n", O_APPEND, O_APPEND);

    const char *f = "/tmp/opencode/flagtest.txt";
    unlink(f);

    mode_t oldmask = umask(0);   /* 读取当前 umask */
    umask(oldmask);              /* 立刻还原，别影响后面 */
    printf("当前 umask = 0%03o\n\n", oldmask);

    /* 1. 创建：请求 0666，实际会被 umask 削减 */
    int fd = open(f, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    struct stat st; fstat(fd, &st);
    printf("[1] O_CREAT|O_TRUNC 请求 0666 -> 实际 0%03o（被 umask 削掉 bits）\n",
           st.st_mode & 0777);
    if (write(fd, "AAA\n", 4) < 0) perror("write");
    close(fd);

    /* 2. O_APPEND：追加到末尾 */
    fd = open(f, O_WRONLY | O_APPEND);
    if (write(fd, "BBB\n", 4) < 0) perror("write");
    close(fd);

    /* 3. 不带 O_APPEND 也不带 O_TRUNC：从偏移 0 覆盖，不截断 */
    fd = open(f, O_WRONLY);
    if (write(fd, "X\n", 2) < 0) perror("write");
    close(fd);

    printf("[2+3] 追加 BBB 后又从头部覆盖 2 字节，文件内容变为：\n");
    fd = open(f, O_RDONLY);
    char buf[64]; ssize_t n = read(fd, buf, sizeof buf - 1); buf[n] = 0;
    close(fd);
    printf("----\n%s----\n\n", buf);

    /* 4. O_EXCL：已存在就原子地失败 */
    errno = 0;
    fd = open(f, O_WRONLY | O_CREAT | O_EXCL, 0644);
    printf("[4] O_CREAT|O_EXCL 打开已存在的文件 -> fd=%d, errno=%s\n",
           fd, strerror(errno));

    unlink(f);
    return 0;
}
