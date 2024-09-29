#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "linux/ioctl.h"
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include "unistd.h"

#include "stdio.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <semaphore.h>

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lvgl/src/extra/libs/lv_libs.h"

/********************************************************************************/
#define error_handler(xx, yy) \
    if (xx < 0)               \
    {                         \
        perror(yy);           \
        return -1;            \
    }
/********************************************************************************/
void *lvgl_ui_task(void *arg);
void ctl_char_io(const char *dev, const char *status);
void lv_btn_callback(lv_event_t *e);
void tabview_btn_cb(lv_event_t *e);
void lvgl_ui(void);

/********************************************************************************/
void *ap3216c_task(void *arg);

struct AP3216C
{
    uint16_t ir;
    uint16_t ps;
    uint16_t als;
};
extern struct AP3216C ap3216c;
/********************************************************************************/
void *network_task(void *arg);
