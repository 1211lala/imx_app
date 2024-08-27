#include "datafile.h"

int main(int argc, char **argv)
{

  // led_thread_thread();
  lcd_show_thread_start();
  uart_task_start();
  printf("主线程结束\r\n");
  pthread_exit(NULL);
}
