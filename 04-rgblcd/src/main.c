#include "main.h"
#include "datafile.h"
#include "jpeg_fun.h"
#include "png_fun.h"

struct _lcddev lcddev = {
    .init = rgblcd_init,
    .drawPonit = lcd_draw_point,
    .showJpg = show_jpeg_image,
    .showPng = show_png_image,
    .delete = lcd_delete,
};

struct PNG_DATA png_data;

u_int32_t *fb_buf_2 = NULL;
u_int32_t *fb_buf_3 = NULL;
int show_image(u_int32_t *buf, struct PNG_DATA *png_data, u_int16_t x, u_int16_t y)
{
    buf = buf + (y * lcddev.width + x);
    for (u_int16_t i = 0; i < png_data->height; i++)
    {
        if (i + y > 599)
        {
            break;
        }
        for (u_int16_t j = 0; j < png_data->width; j++)
        {
            if (j + x > 1023)
            {
                break;
            }
            buf[j] = png_data->data[i * png_data->width + j];
        }
        buf += 1024;
    }
}

int main(int argc, char *argv[])
{

    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
    }
    /* 第二个屏幕buf */
    fb_buf_2 = malloc(lcddev.width * lcddev.height * sizeof(u_int32_t));
    fb_buf_3 = malloc(lcddev.width * lcddev.height * sizeof(u_int32_t));
    memset(lcddev.screenBase, 0xFF, lcddev.screenSize);
    if (0 != png_decode(&png_data, "/p/scr1.png"))
    {
        printf("png_decode failed!!");
    }
    printf("图片长: %d  宽: %d\n", png_data.width, png_data.height);
    static int flag = 0;
    for (u_int16_t i = 0; i < 800; i += 5)
    {
        if (flag == 0)
        {
            flag = 1;
            memset(fb_buf_2, 0xFF, lcddev.screenSize);
            show_image(fb_buf_2, &png_data, i, 0);
            memcpy(lcddev.screenBase, fb_buf_2, lcddev.width * lcddev.height * sizeof(u_int32_t));
        }
        else
        {
            flag = 0;
            memset(fb_buf_3, 0xFF, lcddev.screenSize);
            show_image(fb_buf_3, &png_data, i, 0);
            memcpy(lcddev.screenBase, fb_buf_3, lcddev.width * lcddev.height * sizeof(u_int32_t));
        }
    }

    for (u_int16_t i = 0; i < 600; i += 5)
    {
        if (flag == 0)
        {
            flag = 1;
            memset(fb_buf_2, 0xFF, lcddev.screenSize);
            show_image(fb_buf_2, &png_data, 100, i);
            memcpy(lcddev.screenBase, fb_buf_2, lcddev.width * lcddev.height * sizeof(u_int32_t));
        }
        else
        {
            flag = 0;
            memset(fb_buf_3, 0xFF, lcddev.screenSize);
            show_image(fb_buf_3, &png_data, 100, i);
            memcpy(lcddev.screenBase, fb_buf_3, lcddev.width * lcddev.height * sizeof(u_int32_t));
        }
    }
    free(png_data.data);
    free(fb_buf_2);
    free(fb_buf_3);
    lcddev.delete(&lcddev);
}
