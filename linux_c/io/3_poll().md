### poll()
```C
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);


struct pollfd {
 int fd; /* file descriptor */
 short events; /* requested events */
 short revents; /* returned events */
};
/*
    nfds：参数 nfds 指定了 fds 数组中的元素个数，数据类型 nfds_t 实际为无符号整形

    timeout -1   永久阻塞
    timeout  0   不阻塞
    timeout >0   阻塞 xms
*/

‵‵‵

```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include "poll.h"

#define error_fun(xx, yy) \
    if (xx < 0)           \
    {                     \
        perror(yy);       \
        exit(-1);         \
    }

struct input_event mouse_input;
int mouse_fd, ret;
char buf[100];
fd_set read_set;

struct pollfd fds[2];

int main(void)
{
    /* 设置键盘的FLAG */
    int flag = 0;
    flag = fcntl(0, F_GETFL);
    flag |= O_NONBLOCK;
    ret = fcntl(0, F_SETFL, flag);

    /* 尝试打开事件设备文件，使用正确的设备文件路径 */
    mouse_fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
    error_fun(mouse_fd, "open error");

    while (1)
    {
        /* 同时读取键盘和鼠标 */
        fds[0].fd = 0;
        fds[0].events = POLLIN; //只关心数据可读
        fds[0].revents = 0;

        fds[1].fd = mouse_fd;
        fds[1].events = POLLIN; //只关心数据可读
        fds[1].revents = 0;


        ret = poll(fds, 2, -1);
        if (ret < 0)
        {
            perror("select()");
        }
        else if (ret == 0)
        {
            printf("select timeout\r\n");
        }
            
        if (fds[0].revents & POLLIN)
        {
            ret = read(0, buf, sizeof(buf));
            buf[ret - 1] = '\0';
            printf("键盘读取到: %s\r\n", buf);
        }
        if (fds[1].revents & POLLIN)
        {
            ret = read(mouse_fd, &mouse_input, sizeof(struct input_event));
            printf("type: %d\n", mouse_input.type);
            printf("code: %d\n", mouse_input.code);
            printf("value: %d\n", mouse_input.value);
        }
    }

    /* 关闭文件 */
    close(mouse_fd);
    exit(0);
}

‵‵‵


