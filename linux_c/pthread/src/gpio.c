#include "datafile.h"

#define GPIO_ROOT_PATH "/sys/class/gpio"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"
/****************************************************************
 *
 *
 *
 ****************************************************************/
int gpio_export(unsigned short pin, const char *mode)
{
    int fd;
    char temp[100];
    sprintf(temp, "%s/gpio%d", GPIO_ROOT_PATH, pin);
    if (access(temp, F_OK) < 0)
    {
        fd = open(GPIO_EXPORT_PATH, O_WRONLY);
        if (fd < 0)
        {
            perror(GPIO_EXPORT_PATH);
            goto openerror;
        }
        sprintf(temp, "%d", pin);
        if (strlen(temp) != write(fd, temp, strlen(temp)))
        {
            perror(GPIO_EXPORT_PATH);
            goto writerror;
        }
        close(fd);
    }
    if (strcmp(mode, "out") == 0)
    {
        sprintf(temp, "%s/gpio%d/direction", GPIO_ROOT_PATH, pin);
        fd = open(temp, O_WRONLY);
        if (fd < 0)
        {
            perror(temp);
            goto set_open_error;
        }
        if (strlen(mode) != write(fd, mode, strlen(mode)))
        {
            perror(temp);
            goto set_write_rror;
        }
        close(fd);
        gpio_set_level(pin, "0");
    }
    else if (strcmp(mode, "in") == 0)
    {
    }

    return 0;

set_write_rror:
    close(fd);
set_open_error:
    return -1;
writerror:
    close(fd);
openerror:
    return -1;
}

int gpio_unexport(unsigned short pin)
{
    char temp[100];
    sprintf(temp, "%s/gpio%d", GPIO_ROOT_PATH, pin);
    if (access(temp, F_OK) < 0)
    {
        return 0;
    }
    int fd = open(GPIO_UNEXPORT_PATH, O_WRONLY);
    if (fd < 0)
    {
        perror(GPIO_UNEXPORT_PATH);
        return -1;
    }
    sprintf(temp, "%d", pin);
    write(fd, temp, strlen(temp));
    close(fd);
    return 0;
}
int gpio_set_level(unsigned short pin, const char *status)
{
    int fd;
    char temp[100];
    sprintf(temp, "%s/gpio%d/value", GPIO_ROOT_PATH, pin);
    fd = open(temp, O_WRONLY);
    if (fd < 0)
    {
        perror(temp);
        return -1;
    }
    if (strlen(status) != write(fd, status, strlen(status)))
    {
        close(fd);
        perror(temp);
        return -1;
    }
    close(fd);
    return 0;
}

int gpiO_read_level(void)
{
}