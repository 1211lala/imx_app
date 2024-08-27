#include "stdio.h"
#include <unistd.h>
#include "sys/types.h"
#include <stdlib.h>
#include <pthread.h>
#include "stdio.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define ledpath "../../led/led"
int main(int argc, char **argv)
{
    printf("open led\r\n");
    char buf[100];
    sprintf(buf, "%s %s",ledpath, argv[1]);
    system(buf);
    exit(0);
}