// #include "datafile.h"

#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "led.h"

#define LED_ON 1
#define LED_OFF 0

// #define DEV_PATH "/dev/beep_05_01"
#define DEV_PATH "/dev/led_01_03"


pthread_t led_tid;
int led_thread_thread(void)
{
    printf("led 线程开启\r\n");
    pthread_create(&led_tid, NULL, led_ctrl, NULL);
    pthread_detach(led_tid);
}

void *led_ctrl(void *arg)
{
    int fd = open(DEV_PATH, O_RDWR);
    static char send_buf[1];
    while (1)
    {
        send_buf[0] = LED_ON;
        write(fd, &send_buf[0], 1);
        usleep(100000);
        send_buf[0] = LED_OFF;
        write(fd, &send_buf[0], 1);
        usleep(100000);
    }
    close(fd);
}