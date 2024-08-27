#define _GNU_SOURCE // 在源文件开头定义_GNU_SOURCE宏

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

static int fd = 0;

#define TTY_DEV "/dev/ttymxc2"

/**
 ** 信号处理函数，当串口有数据可读时，会跳转到该函数执行
 **/
static void io_handler(int sig, siginfo_t *info, void *context)
{
    unsigned char buf[10] = {0};
    int ret;
    int n;

    if (SIGRTMIN != sig)
        return;

    /* 判断串口是否有数据可读 */
    if (POLL_IN == info->si_code)
    {
        ret = read(fd, buf, 8); // 一次最多读8个字节数据
        printf("[ ");
        for (n = 0; n < ret; n++)
            printf("0x%hhx ", buf[n]);
        printf("]\n");
    }
}

/**
 ** 异步I/O初始化函数
 **/
static void async_io_init(void)
{
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
}

static pthread_t tip;
static struct termios old_cfg;       // 用于保存终端的配置参数
static struct termios new_cfg = {0}; // 将new_cfg对象清零
unsigned char w_buf[8] = {0x11, 0x22, 0x11, 0x22, 0x11, 0x22, 0x11, 0x22};

void *uart_start(void *arg)
{
    /* 打开 tty 设备 */
    fd = open(TTY_DEV, O_RDWR | O_NOCTTY | O_NONBLOCK);
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
    if (0 > cfsetspeed(&new_cfg, B115200))
    {
        fprintf(stderr, "cfsetspeed error: %s\n", strerror(errno));
        pthread_exit(NULL);
    }

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

#if 0
    async_io_init(); // 我们使用异步I/O方式读取串口的数据，调用该函数去初始化串口的异步I/O
    while (1)
    {
        write(fd, "helloor\n", 8); // 一次向串口写入8个字节
        sleep(1);                  // 进入休眠、等待有数据可读，有数据可读之后就会跳转到io_handler()函数
    }
#else
    u_int8_t buf[100];
    u_int16_t cnt = 0;

    while (1)
    {
        int ret = read(fd, (buf + cnt), 10);
        cnt += ret;
        if (cnt > sizeof(buf))
        {
            printf("数据超出限制\r\n");
        }
        if (ret < 0)
        {
            perror("");
        }
        else if (ret == 0 && cnt > 0)
        { /* 正好接收完处理逻辑 */

            buf[cnt] = '\0';
            printf("接收完: %s\r\n", buf);
            cnt = 0;
        }
        else if (ret > 0 && ret < 10)
        { /* 接收完处理逻辑 */

            buf[cnt] = '\0';
            printf("接收完: %s\r\n", buf);
            cnt = 0;
        }

        static int tt = 0;
        if(tt++  > 100)
        {
            tt = 0;
            write(fd, "12345678\r\n", 10);
            printf("发送完成\r\n");
        }
        
        usleep(10000);
    }
#endif
    /* 退出 */
    tcsetattr(fd, TCSANOW, &old_cfg); // 恢复到之前的配置
    close(fd);
    exit(EXIT_SUCCESS);
}

void uart_task_start(void)
{
    pthread_create(&tip, NULL, uart_start, NULL);
}

#if 0
#define BUFFER_SIZE 256

while (1)
{
    int ret = read(fd, buf + cnt, BUFFER_SIZE - cnt);
    if (ret < 0)
    {
        perror("read error");
        continue;
    }
    else if (ret == 0)
    {
        /* 串口关闭或数据流结束 */
        if (cnt > 0)
        {
            /* 处理剩余的数据 */
            buf[cnt] = '\0'; // 确保字符串结束
            printf("接收完: %s\r\n", buf);
            cnt = 0;
        }
        continue;
    }

    cnt += ret;
    buf[cnt] = '\0'; // 确保字符串结束

    // 假设数据帧以特定字符结束，如 '\n'
    char *end = strchr(buf, '\n');
    if (end != NULL)
    {
        *end = '\0'; // 确保字符串结束
        printf("接收完: %s\r\n", buf);

        // 处理完整的数据帧
        memmove(buf, end + 1, cnt - (end - buf) - 1);
        cnt -= (end - buf) + 1;
    }

    // 发送数据
    write(fd, w_buf, sizeof(w_buf));
    usleep(10000); // 休眠 10 毫秒
}

#endif