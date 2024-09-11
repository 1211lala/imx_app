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

struct IMG_DATA
{
    u_int32_t *data; /* 图片保存的地址 */
    u_int16_t h;     /* 图片的高 */
    u_int16_t w;     /* 图片的宽 */
    u_int8_t bpp;    /* 记录图片的bpp,用于分配内存, 目前未使用 */
};


struct _lcddev
{
    int fd;               /* 屏幕设备的fd文件描述符 */
    int *screenBase;      /* ioctl()为屏幕分配的地址起始地址 */
    u_int32_t screenSize; /* 屏幕内存大小 单位:字节 */
    u_int32_t lineSize;   /* 屏幕一行的字节*/
    u_int16_t height;     /* 屏幕的高度 */
    u_int16_t width;      /* 屏幕的宽度 */
    u_int8_t bpp;         /* 屏幕的像素深度 参考 https://blog.csdn.net/yuejisuo1948/article/details/83617359 */

    int (*init)(struct _lcddev *);                                                            /* 屏幕的初始化函数 */
    void (*drawPonit)(unsigned int, unsigned int, unsigned int);                              /* 屏幕的打点函数函数 */
    int (*showJpg)(struct _lcddev *, const char *, u_int16_t, u_int16_t);                     /* 从文件流中读取数据显示Jpg图片 */
    int (*showPng)(struct _lcddev *, const char *, u_int16_t, u_int16_t);                     /* 从文件流中读取数据显示png图片 */
    int (*showImage)(struct _lcddev *, u_int32_t *, struct IMG_DATA *, u_int16_t, u_int16_t); /* 从内存中读取数据显示图片*/
    void (*delete)(struct _lcddev *);                                                         /* 释放屏幕资源 */
};
extern struct _lcddev lcddev;

int rgblcd_init(struct _lcddev *lcd);
void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color);
int show_image(struct _lcddev *lcd, u_int32_t *showuBuf, struct IMG_DATA *img_data, u_int16_t x, u_int16_t y);
void lcd_delete(struct _lcddev *lcd);