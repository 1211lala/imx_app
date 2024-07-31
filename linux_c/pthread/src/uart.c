#define _GNU_SOURCE     //在源文件开头定义_GNU_SOURCE宏

#include "datafile.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include "uart.h"
#include <signal.h>
#include <termios.h>
#include <fcntl.h>



// typedef struct uart_hardware_cfg
// {
//     unsigned int baudrate; /* 波特率 */
//     unsigned char dbit;    /* 数据位 */
//     char parity;           /* 奇偶校验 */
//     unsigned char sbit;    /* 停止位 */
// } uart_cfg_t;

static int fd = 0;

/**
 ** 信号处理函数，当串口有数据可读时，会跳转到该函数执行
 **/
static void io_handler(int sig, siginfo_t *info, void *context)
{
    unsigned char buf[10] = {0};
    int ret;
    int n;

    if(SIGRTMIN != sig)
        return;

    /* 判断串口是否有数据可读 */
    if (POLL_IN == info->si_code) {
        ret = read(fd, buf, 8);     //一次最多读8个字节数据
        printf("[ ");
        for (n = 0; n < ret; n++)
            printf("0x%hhx ", buf[n]);
        printf("]\n");
    }
}


#define TTY_DEV "/dev/ttymxc1"

static pthread_t tip;
static struct termios old_cfg;       // 用于保存终端的配置参数
static struct termios new_cfg = {0}; // 将new_cfg对象清零
void uart_task_start(void)
{

    pthread_create(&tip, NULL, uart_start, NULL);
}
unsigned char w_buf[8] = {0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 0x11, 0x22};
void *uart_start(void *arg)
{
    
    /* 打开 tty 设备 */
    fd = open(TTY_DEV, O_RDWR | O_NOCTTY);
    if (0 > fd)
    {
        perror(TTY_DEV);
       pthread_exit(NULL);
    }
    if (0 > tcgetattr(fd, &old_cfg))
    {
        perror(TTY_DEV);
        close(fd);
        pthread_exit(NULL);
    }
    /* 设置为原始模式 */
    cfmakeraw(&new_cfg);

    new_cfg.c_cflag |= CREAD;
    new_cfg.c_cflag &= ~CSIZE;  // 将数据位相关的比特位清零
    new_cfg.c_cflag |= CS8;     // 设置8位数据位
    new_cfg.c_cflag &= ~PARENB; // 默认配置为无校验
    new_cfg.c_iflag &= ~INPCK;
    new_cfg.c_cflag &= ~CSTOPB; // 设置一位停止位
    /* 将MIN和TIME设置为0 */
    new_cfg.c_cc[VTIME] = 0;
    new_cfg.c_cc[VMIN] = 0;
    /* 清空缓冲区 */
    if (0 > tcflush(fd, TCIOFLUSH))
    {
        fprintf(stderr, "tcflush error: %s\n", strerror(errno));
        pthread_exit(NULL);
    }
    /* 写入配置、使配置生效 */
    if (0 > tcsetattr(fd, TCSANOW, &new_cfg))
    {
        fprintf(stderr, "tcsetattr error: %s\n", strerror(errno));
        pthread_exit(NULL);
    }
    if (0 > cfsetspeed(&new_cfg, B115200))
    {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        pthread_exit(NULL);
    }

    struct sigaction sigatn;
    int flag;

    /* 使能异步I/O */
    flag = fcntl(fd, F_GETFL); // 使能串口的异步I/O功能
    flag |= O_ASYNC;
    fcntl(fd, F_SETFL, flag);

    /* 设置异步I/O的所有者 */
    fcntl(fd, F_SETOWN, getpid());

    /* 指定实时信号SIGRTMIN作为异步I/O通知信号 */
    fcntl(fd, F_SETSIG, SIGRTMIN);

    /* 为实时信号SIGRTMIN注册信号处理函数 */
    sigatn.sa_sigaction = io_handler; // 当串口有数据可读时，会跳转到io_handler函数
    sigatn.sa_flags = SA_SIGINFO;
    sigemptyset(&sigatn.sa_mask);
    sigaction(SIGRTMIN, &sigatn, NULL);
    /* 读|写串口 */
    switch (1)
    {
    case 0:              // 读串口数据
        async_io_init(); // 我们使用异步I/O方式读取串口的数据，调用该函数去初始化串口的异步I/O
        for (;;)
            sleep(1); // 进入休眠、等待有数据可读，有数据可读之后就会跳转到io_handler()函数
        break;
    case 1: // 向串口写入数据
        
        for (;;)
        {                        // 循环向串口写入数据
            write(fd, w_buf, 8); // 一次向串口写入8个字节
            sleep(1);            // 间隔1秒钟
        }
        break;
    }

    /* 退出 */
    tcsetattr(fd, TCSANOW, &old_cfg); // 恢复到之前的配置
    close(fd);
    exit(EXIT_SUCCESS);
}
