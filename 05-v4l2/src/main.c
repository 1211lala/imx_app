#include "main.h"
#include "datafile.h"
#include "jpeg_fun.h"
#include "png_fun.h"
#include "freetype_fun.h"
#include "v4l2_ov5640.h"

struct _lcddev lcddev = {
    .init = rgblcd_init,
    .drawPonit = lcd_draw_point,
    .showJpg = show_jpeg_image,
    .showPng = show_png_image,
    .showImage = show_image,
    .delete = lcd_delete,
};

struct V4L2_DEV v4l2 = {
    .path = V4L2_PATH,
    .w = 640,
    .h = 480,
};

int main(int argc, char *argv[])
{

    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
        return -1;
    }
    u_int16_t *global_buf_1 = malloc(lcddev.width * lcddev.height * (lcddev.bpp / 8));
    u_int16_t *global_buf_2 = malloc(lcddev.width * lcddev.height * (lcddev.bpp / 8));
    memset(lcddev.screenBase, 0xFF, lcddev.screenSize);
    if (0 != freetype_init("/font/msyhl.ttc", 0))
    {
        printf("freetype_init failed!!");
        return -1;
    }
    v4l2.w = atoi(argv[1]);
    v4l2.h = atoi(argv[2]);
    if (0 != v4l2_dev_init(&v4l2))
    {
        printf("v4l2_dev_init failed!!");
        return -1;
    }
    v4l2_on(&v4l2);
    int8_t flag = 0;
    wchar_t wbuf[10];
    u_int16_t cnt = 0;
    int8_t ccnt = 0;
    while (1)
    {
        if (flag == 1)
        {
            v4l2_get_video(&v4l2, &lcddev, global_buf_1, (1024 - 720) / 2, (600 - 480) / 2);
            memcpy((u_int16_t *)lcddev.screenBase, global_buf_1, lcddev.screenSize);
            memset(global_buf_2, 0xFF, lcddev.screenSize);
            flag = 0;
        }
        else
        {

            v4l2_get_video(&v4l2, &lcddev, global_buf_2, (1024 - 720) / 2, (600 - 480) / 2);
            memcpy((u_int16_t *)lcddev.screenBase, global_buf_2, lcddev.screenSize);
            memset(global_buf_1, 0xFF, lcddev.screenSize);
            flag = 1;
        }
        if (cnt++ > 5)
        {
            cnt = 0;
            swprintf(wbuf, sizeof(wbuf) / sizeof(wbuf[0]), L"%dfps", 30 - rand() % 2);
        }
        lcd_draw_character2(lcddev.screenBase, 900, 30, wbuf, 0xff0000);
        // usleep(10000);
    }
    free(global_buf_1);
    free(global_buf_2);
    lcddev.delete(&lcddev);
}
