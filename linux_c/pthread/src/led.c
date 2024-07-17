#include "datafile.h"

#define LED_TRIGGER "/sys/class/leds/sys-led/trigger"
#define LED_BRIGHTNESS "/sys/class/leds/sys-led/brightness"

int led_set_level(int value)
{
    int fd1, fd2;
    fd1 = open(LED_TRIGGER, O_RDWR);
    if (0 > fd1)
    {
        perror("open error");
        return -1;
    }
    fd2 = open(LED_BRIGHTNESS, O_RDWR);
    if (0 > fd2)
    {
        perror("open error");
        return -1;
    }

    if (value == 1)
    {
        write(fd1, "none", 4); // 先将触发模式设置为 none
        write(fd2, "1", 1);    // 点亮 LED
    }
    else if (value == 0)
    {
        write(fd1, "none", 4); // 先将触发模式设置为 none
        write(fd2, "0", 1);    // LED 灭s
    }
    else
    {
        write(fd1, "heartbeat", strlen("heartbeat"));
    }

    close(fd1);
    close(fd2);
    return 0;
}

/*******************************************************************************************/
/*******************************************************************************************/
/*******************************************************************************************/
pthread_t led_tid;
int led_thread_thread(void)
{
    printf("led 线程开启\r\n");
    pthread_create(&led_tid, NULL, led_ctrl, NULL);
    pthread_detach(led_tid);
}

void *led_ctrl(void *arg)
{
    while (1)
    {
        led_set_level(1);
        usleep(100000);
        led_set_level(0);
        usleep(100000);
    }
}