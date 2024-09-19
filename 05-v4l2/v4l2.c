#include "main.h"
#include "datafile.h"
#include "jpeg_fun.h"
#include "png_fun.h"
#include "freetype_fun.h"
#include <linux/videodev2.h>

#define Video0_Path "/dev/video1"

#define FRAMEBUFFER_COUNT 3
struct _lcddev lcddev = {
    .init = rgblcd_init,
    .drawPonit = lcd_draw_point,
    .showJpg = show_jpeg_image,
    .showPng = show_png_image,
    .showImage = show_image,
    .delete = lcd_delete,
};

typedef struct cam_buf_info
{
    unsigned short *start; // 帧缓冲起始地址
    unsigned long length;  // 帧缓冲长度
} cam_buf_info;
static cam_buf_info buf_infos[FRAMEBUFFER_COUNT];

static int frm_width, frm_height; // 视频帧宽度和高度
static u_int16_t width = 1024, height = 600;
struct v4l2_capability cap = {0};
int v4l2_fd = 0;
int v4l2_dev_init(const char *path)
{
    /* 打开摄像头 */
    v4l2_fd = open(path, O_RDWR);
    if (0 > v4l2_fd)
    {
        fprintf(stderr, "open error: %s: %s\n", path, strerror(errno));
        return -1;
    }
    ioctl(v4l2_fd, VIDIOC_QUERYCAP, &cap);
    /* 判断是否是视频采集设备 */
    if (!(V4L2_CAP_VIDEO_CAPTURE & cap.capabilities))
    {
        fprintf(stderr, "Error: %s: No capture video device!\n", path);
        close(v4l2_fd);
        return -1;
    }
    return 0;
}

/* 枚举出摄像头支持的所有像素格式：VIDIOC_ENUM_FMT */
struct v4l2_fmtdesc fmtdesc;
int v4l2_enum_fmt(void)
{
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("%-20s%-20s", "像素格式", "像素格式编号\n");
    while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FMT, &fmtdesc))
    {
        printf("%-20s 0x%-20x\n", fmtdesc.description, fmtdesc.pixelformat);
        fmtdesc.index += 1;
    }
    printf("\n");
    return 0;
}
/* 枚举摄像头所支持的所有视频采集分辨率 VIDIOC_ENUM_FRAMESIZES*/
struct v4l2_frmsizeenum fresize;
int v4l2_enum_framesize(void)
{
    fresize.index = 0;
    fresize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fresize.pixel_format = V4L2_PIX_FMT_RGB565;
    printf("%s\n", "像素格式RGB565支持的分辨率大小");
    while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FRAMESIZES, &fresize))
    {
        printf("size %d * %d\n", fresize.discrete.width, fresize.discrete.height);
        fresize.index += 1;
    }
    fresize.index = 0;
    fresize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fresize.pixel_format = V4L2_PIX_FMT_JPEG;
    printf("%s\n", "像素格式JPEG支持的分辨率大小");
    while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FRAMESIZES, &fresize))
    {
        printf("size %d * %d\n", fresize.discrete.width, fresize.discrete.height);
        fresize.index += 1;
    }
    fresize.index = 0;
    fresize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fresize.pixel_format = V4L2_PIX_FMT_YUYV;
    printf("%s\n", "像素格式YUYV支持的分辨率大小");
    while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FRAMESIZES, &fresize))
    {
        printf("size %d * %d\n", fresize.discrete.width, fresize.discrete.height);
        fresize.index += 1;
    }
    printf("\n");
    return 0;
}
/* 枚举摄像头所支持的所有视频采集帧率：VIDIOC_ENUM_FRAMEINTERVALS */
struct v4l2_frmivalenum frmival;
int v4l2_enum_fps(void)
{
    frmival.index = 0;
    frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frmival.pixel_format = V4L2_PIX_FMT_RGB565;
    frmival.height = 600;
    frmival.width = 1024;
    printf("RGB565 600*1024支持的帧率\n");
    while (0 == ioctl(v4l2_fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival))
    {
        printf("fps %d\n", frmival.discrete.denominator / frmival.discrete.numerator);
        frmival.index += 1;
    }
}

int v4l2_set_format(u_int16_t w, u_int16_t h)
{
    struct v4l2_format fmt = {0};

    /* 设置帧格式 */
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;        // type 类型
    fmt.fmt.pix.width = w;                         // 视频帧宽度
    fmt.fmt.pix.height = h;                        // 视频帧高度
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565; // 像素格式
    if (0 > ioctl(v4l2_fd, VIDIOC_S_FMT, &fmt))
    {
        fprintf(stderr, "ioctl error: VIDIOC_S_FMT: %s\n", strerror(errno));
        return -1;
    }
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB565)
    {
        fprintf(stderr, "此设备不支持RGB565\n");
        return -1;
    }
    frm_width = fmt.fmt.pix.width;   // 获取实际的帧宽度
    frm_height = fmt.fmt.pix.height; // 获取实际的帧高度
    printf("视频帧大小<%d * %d>\n", frm_width, frm_height);

    /* 设置帧格式 */
    struct v4l2_streamparm streamparm = {0};
    /* 获取 streamparm */
    streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(v4l2_fd, VIDIOC_G_PARM, &streamparm);
    /** 判断是否支持帧率设置 **/
    if (V4L2_CAP_TIMEPERFRAME & streamparm.parm.capture.capability)
    {
        streamparm.parm.capture.timeperframe.numerator = 1;
        streamparm.parm.capture.timeperframe.denominator = 30; // 30fps
        if (0 > ioctl(v4l2_fd, VIDIOC_S_PARM, &streamparm))
        {
            fprintf(stderr, "ioctl error: VIDIOC_S_PARM: %s\n", strerror(errno));
            return -1;
        }
    }
    return 0;
}
static int v4l2_stream_on(void)
{
    /* 打开摄像头、摄像头开始采集数据 */
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(v4l2_fd, VIDIOC_STREAMON, &type))
    {
        fprintf(stderr, "ioctl error: VIDIOC_STREAMON: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int v4l2_init_buffer(void)
{
    /* 向内存申请帧缓存 */
    struct v4l2_requestbuffers reqbuf = {0};
    reqbuf.count = FRAMEBUFFER_COUNT;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(v4l2_fd, VIDIOC_REQBUFS, &reqbuf))
    {
        perror("");
        return -1;
    }
    /* 擦获取帧缓存的信息 */
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
    {
        ioctl(v4l2_fd, VIDIOC_QUERYBUF, &buf);
        buf_infos[buf.index].length = buf.length;
        /* 设置内存映射 */
        buf_infos[buf.index].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, v4l2_fd, buf.m.offset);
        printf("addr%d: %x  len: %x\n", buf.index, buf_infos[buf.index].start, buf_infos[buf.index].length);
        if (MAP_FAILED == buf_infos[buf.index].start)
        {
            perror("mmap error");
            return -1;
        }
    }
    printf("-2\r\n");
    /* 入队 */
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
    {
        if (0 > ioctl(v4l2_fd, VIDIOC_QBUF, &buf))
        {
            fprintf(stderr, "ioctl error: VIDIOC_QBUF: %s\n", strerror(errno));
            return -1;
        }
    }
    printf("-=1\r\n");
    return 0;
}
void v4l2_read_data(void)
{
    struct v4l2_buffer buf = {0};
    unsigned short *base;
    unsigned short *start;
    int min_w, min_h;
    int j;
    if (width > frm_width)
        min_w = frm_width;
    else
        min_w = width;
    if (height > frm_height)
        min_h = frm_height;
    else
        min_h = height;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    printf("min_w: %d min_h: %d\n", min_w, min_h);
    while (1)
    {
        for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
        {
            ioctl(v4l2_fd, VIDIOC_DQBUF, &buf); // 出队
            base = (unsigned short *)lcddev.screenBase;
            start = buf_infos[buf.index].start;
            for (j = 0; j < min_h; j++)
            {

                memcpy(base, start, min_w * 2); // RGB565 一个像素占 2 个字节
                base += width;                  // LCD 显示指向下一行
                start += frm_width;             // 指向下一行数据
            }
            // 数据处理完之后、再入队、往复
            ioctl(v4l2_fd, VIDIOC_QBUF, &buf);
        }
    }
}
int main(int argc, char *argv[])
{
    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
        return -1;
    }
    memset(lcddev.screenBase, 0x89, lcddev.screenSize);
    v4l2_dev_init(Video0_Path);
    v4l2_enum_fmt();
    v4l2_enum_framesize();
    v4l2_enum_fps();
    v4l2_set_format(640, 480);
    v4l2_init_buffer();
    printf("0\r\n");
    v4l2_stream_on();
    printf("1\r\n");
    v4l2_read_data();
    lcddev.delete(&lcddev);
}
