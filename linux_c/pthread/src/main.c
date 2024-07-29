#include "datafile.h"

int main(int argc, char **argv)
{
    // lcd_show_thread_start();
    // led_thread_thread();
    uart_task_start();
    gpio_unexport(2);
    gpio_unexport(4);
    gpio_unexport(8);
    gpio_export(4, "out");
    while (1)
    {
        gpio_set_level(4, "0");
        usleep(10000);
        gpio_set_level(4, "1");
        usleep(100000);
    }
    printf("主线程结束\r\n");
    pthread_exit(NULL);
}
