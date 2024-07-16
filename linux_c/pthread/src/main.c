#include "main.h"
#include "stdio.h"
#include <unistd.h>
#include "sys/types.h"
#include <stdlib.h>
#include <pthread.h>
#include "stdio.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>





pthread_t ppid1;
pthread_t ppid2;
pthread_attr_t attr;

void *thread_fun1(void *arg)
{
    printf("进程ID: %d     线程ID: %ld  开启\r\n", getpid(), ppid1);
    while (1)
    {
        printf("thread_fun1 app runing\r\n");
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    if (pthread_create(&ppid1, NULL, thread_fun1, NULL) != 0)
    {
        printf("thread_fun1 create failed!!!\r\n");
        exit(-1);
    }
    sleep(3);
    printf("开始取消任务\r\n");
    pthread_cancel(ppid1);

    void *ret;
    /*  因为调用 pthread_cancel() 对面传过来的只是一个数字, 所以不用 (void *)&ret 
        是直接把 PTHREAD_CANCELED 这个值 放在了 ret 这个指针变量中
    */
    pthread_join(ppid1, (void *)&ret);
    if (ret == PTHREAD_CANCELED)
        printf("取消成功\r\n");
    else
        printf("取消失败\r\n");
    pthread_exit(NULL);
}