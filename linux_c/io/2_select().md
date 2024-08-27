## IO的多路复用

### slect()
系统调用 select()可用于执行 I/O 多路复用操作，调用 select()会一直阻塞，直到某一个或多个文件描述符成为就绪态（可以读或写）
```C

#include <sys/select.h>
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

```

select()函数将阻塞知道有以下事情发生：  
readfds、writefds 或 exceptfds 指定的文件描述符中至少有一个称为就绪态；  
该调用被信号处理函数中断；  
参数 timeout 中指定的时间上限已经超时。  



select()函数的返回值  
select()函数有三种可能的返回值，会返回如下三种情况中的一种：  
返回-1 表示有错误发生，并且会设置 errno。可能的错误码包括 EBADF、EINTR、EINVAL、EINVAL  以及 ENOMEM，EBADF 表示 readfds、writefds 或 exceptfds 中有一个文件描述符是非法的；EINTR 表示该函数被信号处理函数中断了

返回 0 表示在任何文件描述符成为就绪态之前 select()调用已经超时，在这种情况下，readfds，   writefds 以及 exceptfds 所指向的文件描述符集合都会被清空。    

返回一个正整数表示有一个或多个文件描述符已达到就绪态。返回值表示处于就绪态的文件描述符的个数，在这种情况下，每个返回的文件描述符集合都需要检查，通过 FD_ISSET()宏进行检查，
以此找出发生的 I/O 事件是什么。如果同一个文件描述符在 readfds，writefds 以及   exceptfds 中同时被指定，且它多于多个 I/O 事件都处于就绪态的话，那么就会被统计多次，换句话说




对 fd_set 类型进行操作
```C

void FD_ZERO(fd_set *set);          //将参数 set 所指向的集合初始化为空；
void FD_CLR(int fd, fd_set *set);   //将文件描述符 fd 从参数 set 所指向的集合中移除；
void FD_SET(int fd, fd_set *set);   //将文件描述符 fd 添加到参数 set 所指向的集合中；
int FD_ISSET(int fd, fd_set *set);  // 如果文件描述符 fd 是参数 set 所指向的集合中的成员，则 FD_ISSET()返回 true，否则返回 false
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
#include <sys/select.h>

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

        // FD_ZERO(&read_set);
        FD_SET(0, &read_set);
        FD_SET(mouse_fd, &read_set);

        ret = select(mouse_fd + 1, &read_set, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select()");
        }
        else if (ret == 0)
        {
            printf("select timeout\r\n");
        }

        if (FD_ISSET(0, &read_set))
        {
            ret = read(0, buf, sizeof(buf));
            buf[ret - 1] = '\0';
            printf("键盘读取到: %s\r\n", buf);
        }
        if (FD_ISSET(mouse_fd, &read_set))
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