#pragma once


#include "datafile.h"


#define FRAMEBUFFER_COUNT 3
#define V4L2_PATH "/dev/video1"

typedef struct CAMERA_BUF_INFO
{
    u_int16_t *start; /* 起始地址 */
    u_int32_t length; /* 内存块大小 */
} cam_buf_info;

struct V4L2_DEV
{
    int fd;
    const char *path;
    u_int16_t w;                         /* V4L2画面宽度 */
    u_int16_t h;                         /* V4L2画面高度 */
    cam_buf_info buf[FRAMEBUFFER_COUNT]; /* V4L2帧缓存 */
};
extern struct V4L2_DEV v4l2;

int v4l2_dev_init(struct V4L2_DEV *v4l2dev);
int v4l2_on(struct V4L2_DEV *v4l2dev);
void v4l2_show_video(struct V4L2_DEV *v4l2dev, struct _lcddev *lcd, u_int16_t *showbuf, u_int16_t x, u_int16_t y);
int v4l2_get_video(struct V4L2_DEV *v4l2dev, struct _lcddev *lcd, u_int16_t *tempbuf, u_int16_t x, u_int16_t y);