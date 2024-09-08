#include "main.h"
#include "datafile.h"
#include <jpeglib.h>
#include <setjmp.h>
typedef struct bgr888_color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} __attribute__((packed)) bgr888_t;

static int show_jpeg_image(struct _lcddev *lcd, const char *path);

int main(int argc, char *argv[])
{
    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
    }
    while (1)
    {
        memset(lcddev.screenBase, 0xFF, lcddev.screenSize);
        show_jpeg_image(&lcddev, "/p/test.jpg");
        sleep(1);
        memset(lcddev.screenBase, 0xFF, lcddev.screenSize);
        show_jpeg_image(&lcddev, "/p/test2.jpg");
        sleep(1);
    }

    munmap(lcddev.screenBase, lcddev.screenSize);
    close(lcddev.fd);
    printf("lcd memery release success!!!\n");
    exit(0);

    // lcddev.delete(&lcddev);
    // return 0;
}

static int show_jpeg_image(struct _lcddev *lcd, const char *path)
{
    u_int32_t *showAddr = NULL;

    showAddr = lcd->screenBase;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *jpeg_file = NULL;
    bgr888_t *jpeg_line_buf = NULL;     // 行缓冲区:用于存储从 jpeg 文件中解压出来的一行图像数据
    unsigned short *fb_line_buf = NULL; // 行缓冲区:用于存储写入到 LCD 显存的一行数据
    unsigned int min_h, min_w;
    unsigned int valid_bytes;
    int i;
    // 绑定默认错误处理函数
    cinfo.err = jpeg_std_error(&jerr);
    // 打开.jpeg/.jpg 图像文件
    jpeg_file = fopen(path, "r"); // 只读方式打开
    if (NULL == jpeg_file)
    {
        perror("fopen error ");
        return -1;
    }
    // 创建 JPEG 解码对象
    jpeg_create_decompress(&cinfo);
    // 指定图像文件
    jpeg_stdio_src(&cinfo, jpeg_file);
    // 读取图像信息
    jpeg_read_header(&cinfo, TRUE);

    printf("jpeg 图像大小: %d*%d\n", cinfo.image_width, cinfo.image_height);
    // 设置解码参数
    cinfo.out_color_space = JCS_RGB; // 默认就是 JCS_RGB
    // cinfo.scale_num = 1;
    // cinfo.scale_denom = 2;
    // 开始解码图像
    jpeg_start_decompress(&cinfo);
    // 为缓冲区分配内存空间
    jpeg_line_buf = malloc(cinfo.output_components * cinfo.output_width);
    printf("cinfo.output_components: %d\n", cinfo.output_components);
    fb_line_buf = malloc(lcd->lineSize);
    // 判断图像和 LCD 屏那个的分辨率更低
    if (cinfo.output_width > lcd->width)
        min_w = lcd->width;
    else
        min_w = cinfo.output_width;
    if (cinfo.output_height > lcd->height)
        min_h = lcd->height;
    else
        min_h = cinfo.output_height;
    // 读取数据
    valid_bytes = min_w * lcd->bpp / 8; // 一行的有效字节数 表示真正写入到 LCD 显存的一行数据的大小

    // lcddev.screenBase += 100 * lcddev.width + 100;
    while (cinfo.output_scanline < min_h)
    {
        jpeg_read_scanlines(&cinfo, (unsigned char **)&jpeg_line_buf, 1); // 每次读取一行数据
        // 将读取到的 BGR888 数据转为 RGB565
        for (i = 0; i < min_w; i++)
            fb_line_buf[i] = ((jpeg_line_buf[i].red & 0xF8) << 8) |
                             ((jpeg_line_buf[i].green & 0xFC) << 3) |
                             ((jpeg_line_buf[i].blue & 0xF8) >> 3);
        for (i = 0; i < min_w; i++)
        {
            u_int32_t color = jpeg_line_buf[i].red << 16 | jpeg_line_buf[i].green << 8 | jpeg_line_buf[i].blue;
            lcd->screenBase[i] = color;
        }
        lcd->screenBase += lcd->width; //+width 定位到 LCD 下一行显存地址的起点
    }
    lcd->screenBase = showAddr;
    // 解码完成
    jpeg_finish_decompress(&cinfo);  // 完成解码
    jpeg_destroy_decompress(&cinfo); // 销毁 JPEG 解码对象、释放资源
    // 关闭文件、释放内存
    fclose(jpeg_file);
    free(fb_line_buf);
    free(jpeg_line_buf);
    return 0;
}
