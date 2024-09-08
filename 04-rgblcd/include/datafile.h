#pragma once

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "linux/fb.h"
#include <unistd.h>

// #define USE_VER_SCREEN
struct _lcddev
{
    int fd;
    int *screenBase;
    u_int32_t screenSize;
    u_int32_t lineSize;
    u_int16_t height;
    u_int16_t width;
    u_int8_t bpp;

    int (*init)(struct _lcddev *);
    void (*drawPonit)(unsigned int, unsigned int, unsigned int);
    void (*delete)(struct _lcddev *);
};
extern struct _lcddev lcddev;

int rgblcd_init(struct _lcddev *lcd);
void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color);
void lcd_delete(struct _lcddev *lcd);