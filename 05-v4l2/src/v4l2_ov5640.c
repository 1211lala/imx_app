#include "v4l2_ov5640.h"
#include <linux/videodev2.h>
#include "freetype_fun.h"

int v4l2_dev_init(struct V4L2_DEV *v4l2dev)
{
    /* 打开摄像头设备 */
    v4l2dev->fd = open(v4l2dev->path, O_RDWR);
    if (0 > v4l2dev->fd)
    {
        fprintf(stderr, "open error: %s: %s\n", v4l2dev->path, strerror(errno));
        return -1;
    }
    /* 判断是否是视频采集设备 V4L2_CAP_VIDEO_CAPTURE */
    struct v4l2_capability cap = {0};
    ioctl(v4l2dev->fd, VIDIOC_QUERYCAP, &cap);
    if (!(V4L2_CAP_VIDEO_CAPTURE & cap.capabilities))
    {
        fprintf(stderr, "Error: %s: No capture video device!\n", v4l2dev->path);
        close(v4l2dev->fd);
        return -1;
    }
    /* 枚举出摄像头支持的所有像素格式：VIDIOC_ENUM_FMT */
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("%-20s%-20s", "像素格式", "像素格式编号\n");
    while (0 == ioctl(v4l2dev->fd, VIDIOC_ENUM_FMT, &fmtdesc))
    {
        printf("%-20s 0x%-20x\n", fmtdesc.description, fmtdesc.pixelformat);
        fmtdesc.index += 1;
    }
    printf("\n");
    /* 枚举摄像头所支持的所有视频采集分辨率 VIDIOC_ENUM_FRAMESIZES */
    struct v4l2_frmsizeenum fresize = {0};
    fresize.index = 0;
    fresize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fresize.pixel_format = V4L2_PIX_FMT_RGB565;
    printf("%s\n", "像素格式RGB565支持的分辨率大小");
    while (0 == ioctl(v4l2dev->fd, VIDIOC_ENUM_FRAMESIZES, &fresize))
    {
        printf("size %d * %d\n", fresize.discrete.width, fresize.discrete.height);
        fresize.index += 1;
    }
    fresize.index = 0;
    fresize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fresize.pixel_format = V4L2_PIX_FMT_JPEG;
    printf("%s\n", "像素格式JPEG支持的分辨率大小");
    while (0 == ioctl(v4l2dev->fd, VIDIOC_ENUM_FRAMESIZES, &fresize))
    {
        printf("size %d * %d\n", fresize.discrete.width, fresize.discrete.height);
        fresize.index += 1;
    }
    fresize.index = 0;
    fresize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fresize.pixel_format = V4L2_PIX_FMT_YUYV;
    printf("%s\n", "像素格式YUYV支持的分辨率大小");
    while (0 == ioctl(v4l2dev->fd, VIDIOC_ENUM_FRAMESIZES, &fresize))
    {
        printf("size %d * %d\n", fresize.discrete.width, fresize.discrete.height);
        fresize.index += 1;
    }
    printf("\n");
    /* 枚举摄像头所支持的所有视频采集帧率：VIDIOC_ENUM_FRAMEINTERVALS */
    struct v4l2_frmivalenum frmival = {0};
    frmival.index = 0;
    frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frmival.pixel_format = V4L2_PIX_FMT_RGB565;
    frmival.height = 600;
    frmival.width = 1024;
    printf("RGB565 600*1024支持的帧率\n");
    while (0 == ioctl(v4l2dev->fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival))
    {
        printf("fps %d\n", frmival.discrete.denominator / frmival.discrete.numerator);
        frmival.index += 1;
    }
    /* 设置摄像头采集的帧格式:VIDIOC_S_FMT */
    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;        // type 类型
    fmt.fmt.pix.width = v4l2dev->w;                // 视频帧宽度
    fmt.fmt.pix.height = v4l2dev->h;               // 视频帧高度
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB565; // 像素格式
    if (0 > ioctl(v4l2dev->fd, VIDIOC_S_FMT, &fmt))
    {
        fprintf(stderr, "ioctl error: VIDIOC_S_FMT: %s\n", strerror(errno));
        return -1;
    }
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB565)
    {
        fprintf(stderr, "此设备不支持RGB565\n");
        return -1;
    }
    v4l2dev->w = fmt.fmt.pix.width;  // 获取实际的帧宽度
    v4l2dev->h = fmt.fmt.pix.height; // 获取实际的帧高度
    printf("视频帧大小<%d * %d>\n", v4l2dev->w, v4l2dev->h);
    /* 设置摄像头采集的帧频率:VIDIOC_G_PARM */
    struct v4l2_streamparm streamparm = {0};
    streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(v4l2dev->fd, VIDIOC_G_PARM, &streamparm);
    if (V4L2_CAP_TIMEPERFRAME & streamparm.parm.capture.capability) /** 判断是否支持帧率设置 **/
    {
        streamparm.parm.capture.timeperframe.numerator = 1;
        streamparm.parm.capture.timeperframe.denominator = 30; // 30fps
        if (0 > ioctl(v4l2dev->fd, VIDIOC_S_PARM, &streamparm))
        {
            fprintf(stderr, "ioctl error: VIDIOC_S_PARM: %s\n", strerror(errno));
            return -1;
        }
    }
    /* 向内核申请帧buf 并映射到用户内存  */
    struct v4l2_requestbuffers reqbuf = {0};
    reqbuf.count = FRAMEBUFFER_COUNT;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(v4l2dev->fd, VIDIOC_REQBUFS, &reqbuf))
    {
        perror("");
        return -1;
    }
    /* 获取帧缓存的信息 */
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
    {
        ioctl(v4l2dev->fd, VIDIOC_QUERYBUF, &buf);
        v4l2dev->buf[buf.index].length = buf.length;
        /* 设置内存映射 */
        v4l2dev->buf[buf.index].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, v4l2dev->fd, buf.m.offset);
        printf("addr%d: %x  len: %x\n", buf.index, v4l2dev->buf[buf.index].start, v4l2dev->buf[buf.index].length);
        if (MAP_FAILED == v4l2dev->buf[buf.index].start)
        {
            perror("mmap error");
            return -1;
        }
    }
    /* 入队 */
    for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
    {
        if (0 > ioctl(v4l2dev->fd, VIDIOC_QBUF, &buf))
        {
            fprintf(stderr, "ioctl error: VIDIOC_QBUF: %s\n", strerror(errno));
            return -1;
        }
    }
    return 0;
}

int v4l2_on(struct V4L2_DEV *v4l2dev)
{
    /* 打开摄像头、摄像头开始采集数据 */
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (0 > ioctl(v4l2dev->fd, VIDIOC_STREAMON, &type))
    {
        fprintf(stderr, "ioctl error: VIDIOC_STREAMON: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void v4l2_show_video(struct V4L2_DEV *v4l2dev, struct _lcddev *lcd, u_int16_t *showbuf, u_int16_t x, u_int16_t y)
{
    struct v4l2_buffer buf = {0};
    unsigned short *base;
    unsigned short *start;
    int min_w, min_h;
    if (lcd->width > v4l2dev->w)
        min_w = v4l2dev->w;
    else
        min_w = lcd->width;
    if (lcd->height > v4l2dev->h)
        min_h = v4l2dev->h;
    else
        min_h = lcd->height;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    printf("min_w: %d min_h: %d\n", min_w, min_h);
    while (1)
    {
        for (buf.index = 0; buf.index < FRAMEBUFFER_COUNT; buf.index++)
        {
            ioctl(v4l2dev->fd, VIDIOC_DQBUF, &buf);
            base = (unsigned short *)showbuf + (y * lcd->width + x);
            start = v4l2dev->buf[buf.index].start;

            for (int j = 0; j < min_h; j++)
            {
                if (y + j > (lcd->height - 1))
                    break;
                if (x + min_w > (lcd->width - 1))
                    memcpy(base, start, (lcd->width - x) * 2); // RGB565 一个像素占 2 个字节
                else
                    memcpy(base, start, min_w * 2); // RGB565 一个像素占 2 个字节

                base += lcd->width;  // LCD 显示指向下一行
                start += v4l2dev->w; // 指向下一行数据
            }
            ioctl(v4l2dev->fd, VIDIOC_QBUF, &buf); // 数据处理完之后、再入队、往复
        }
    }
}

int v4l2_get_video(struct V4L2_DEV *v4l2dev, struct _lcddev *lcd, u_int16_t *tempbuf, u_int16_t x, u_int16_t y)
{
    static int buf_cnt = 1;

    struct v4l2_buffer buf = {0};

    int min_w, min_h;
    if (lcd->width > v4l2dev->w)
        min_w = v4l2dev->w;
    else
        min_w = lcd->width;
    if (lcd->height > v4l2dev->h)
        min_h = v4l2dev->h;
    else
        min_h = lcd->height;

    if (buf_cnt > FRAMEBUFFER_COUNT - 1)
        buf_cnt = 0;
    buf.index = buf_cnt;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    // printf("buf_cnt: %d min_w: %d min_h: %d\n", buf_cnt, min_w, min_h);
    ioctl(v4l2dev->fd, VIDIOC_DQBUF, &buf);
    unsigned short *baseaddr = tempbuf + (y * lcd->width + x);
    unsigned short *start = v4l2.buf[buf_cnt].start;
    for (int j = 0; j < min_h; j++)
    {
        if (y + j > (lcd->height - 1))
            break;
        if (x + min_w > (lcd->width - 1))
            memcpy(baseaddr, start, (lcd->width - x) * 2); // RGB565 一个像素占 2 个字节
        else
            memcpy(baseaddr, start, min_w * 2); // RGB565 一个像素占 2 个字节

        baseaddr += lcd->width; // LCD 显示指向下一行
        start += v4l2dev->w;    // 指向下一行数据
    }
    ioctl(v4l2dev->fd, VIDIOC_QBUF, &buf); // 数据处理完之后、再入队、往复
    buf_cnt += 1;
    return 0;
}