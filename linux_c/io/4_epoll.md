## epoll()

### 触发方式
    1. 边缘触发
    2. 状态触发

###


```C
typedef union epoll_data /* 用户数据变量 */
{
  void *ptr;
  int fd;   
  uint32_t u32;
  uint64_t u64;
} epoll_data_t; 

struct epoll_event
{
  uint32_t events;	    /* Epoll 事件 */
  epoll_data_t data;	/* 用户数据变量 */
} __EPOLL_PACKED;


// 创建一个 epoll 实例，并返回一个 epoll 文件描述符
int epoll_create1 (int __flags)
// 操作函数
/*
  __epfd : epoll实例的文件描述符
  __op  : EPOLL_CTL_ADD // 将fd添加到epoll中
          EPOLL_CTL_DEL // 将epoll中的fd删除
          EPOLL_CTL_MOD // 将fd的原来的 __event 修改为 新的 __event
  __fd : 监听的文件描符
  __event ：关联的描述信息
*/
int epoll_ctl (int __epfd, int __op, int __fd, struct epoll_event *__event)

/*
  __epfd : epoll实例的文件描述符
  __events 提供给内核用于返回已就绪的文件描述符信息
  __maxevents 可以返回的最大的文件描述符的数量
  __timeout 超时时间 
                    -1  永久等待
                     0  不等待
                     x  等待xms
*/
int epoll_wait (int __epfd, struct epoll_event *__events,int __maxevents, int __timeout);


```


```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include "sys/epoll.h"

#define error_fun(xx, yy) \
    if (xx < 0)           \
    {                     \
        perror(yy);       \
        exit(-1);         \
    }

struct input_event mouse_input;

int ret;

char buf[100];

int main(void)
{
    /* 设置键盘的FLAG */
    int flag = 0;
    flag = fcntl(0, F_GETFL);
    flag |= O_NONBLOCK;
    ret = fcntl(0, F_SETFL, flag);

    /* 尝试打开事件设备文件，使用正确的设备文件路径 */
    int mouse_fd = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
    error_fun(mouse_fd, "open error");

    struct epoll_event epv, rec_epv[2];
    int epfd = epoll_create1(0);

    epv.data.fd = mouse_fd;
    epv.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, mouse_fd, &epv);

    epv.data.fd = 0;
    epv.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &epv);

    while (1)
    {
        ret = epoll_wait(epfd, rec_epv, 2, -1);
        if (ret < 0)
        {
            perror("select()");
        }
        else if (ret == 0)
        {
            printf("select timeout\r\n");
        }

        for (int i = 0; i < ret; i++)
        {
            if (rec_epv[i].data.fd == mouse_fd)
            {
                ret = read(mouse_fd, &mouse_input, sizeof(struct input_event));
                if (ret <= 0)
                {
                    continue;
                }
                printf("type: %d\n", mouse_input.type);
                printf("code: %d\n", mouse_input.code);
                printf("value: %d\n", mouse_input.value);
            }
            else if (rec_epv[i].data.fd == 0)
            {
                ret = read(0, buf, sizeof(buf));
                if (ret <= 0)
                {
                    continue;
                }
                buf[ret - 1] = '\0';
                printf("键盘读取到: %s\r\n", buf);
            }
        }
    }

    /* 关闭文件 */
    close(mouse_fd);
    exit(0);
}

‵``