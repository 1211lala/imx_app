#include "datafile.h"
#include "semaphore.h"

int main(int argc, char **argv)
{
    // lcd_show_thread_start();
    // led_thread_thread();

    printf("主线程结束\r\n");
    pthread_exit(NULL);
}
