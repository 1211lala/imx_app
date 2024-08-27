#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <linux/input.h>

#define error_fun(xx, yy) \
    if (xx < 0)           \
    {                     \
        perror(yy);       \
        exit(-1);         \
    }

static int devfd, ret;
struct input_event dev_input;
char buf[100] = {0};

static void sigio_handler(int sig)
{
    if (SIGIO != sig)
        return;
    if (dev_input.type == EV_MSC)
    {
        printf("键盘事件:code=%d, value=%d\n",dev_input.code, dev_input.value);
    }
}

int main(void)
{
    devfd = open("/dev/input/event1", O_RDONLY);
    error_fun(devfd, "open error");

    /* 使能键盘文件异步IO 与非阻塞IO*/
    int flag = fcntl(devfd, F_GETFL);
    flag |= O_ASYNC | O_NONBLOCK;
    fcntl(devfd, F_SETFL, flag);

    /* 设置异步 I/O 的所有者 */
    fcntl(devfd, F_SETOWN, getpid());

    signal(SIGIO, sigio_handler);

    while (1)
    {

    }
}