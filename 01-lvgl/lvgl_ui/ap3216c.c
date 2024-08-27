#include "datafile.h"

#define AP3216C_PATH "/dev/ap3216c"

static int ap3216c_fd;
static int ret;

struct AP3216C ap3216c;

void *ap3216c_task(void *arg)
{
    unsigned short ap3216c_buf[3];
    ap3216c_fd = open(AP3216C_PATH, O_RDWR);
    error_handler(ap3216c_fd, "ap3216c open");
    while (1)
    {
        ret = read(ap3216c_fd, ap3216c_buf, sizeof(ap3216c_buf));
        error_handler(ret, "ap3216c read");
        ap3216c.als = ap3216c_buf[2];
        ap3216c.ps = ap3216c_buf[1];
        ap3216c.ir = ap3216c_buf[0];
        
        usleep(100000);
        // printf("user ir: %d ps: %d  als: %d\r\n", ap3216c_buf[0], ap3216c_buf[1], ap3216c_buf[2]);
        
    }
    close(ap3216c_fd);
    pthread_exit(NULL);
}
