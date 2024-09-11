###

```C
#include "main.h"
#include "datafile.h"
// #include "jpeg_fun.h"
#include "png_fun.h"

struct _lcddev lcddev = {
    .init = rgblcd_init,
    .drawPonit = lcd_draw_point,
    // .showJpg = show_jpeg_image,
    .showPng = show_png_image,
    .showImage = show_image,
    .delete = lcd_delete,
};
struct IMG_DATA img_data;
u_int32_t *fb_buf_1 = NULL;
u_int32_t *fb_buf_2 = NULL;

int main(int argc, char *argv[])
{
    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
    }
    memset(lcddev.screenBase, 0x00, lcddev.screenSize);
    /* 添加两个新的显示buf */
    fb_buf_1 = malloc(lcddev.width * lcddev.height * sizeof(u_int32_t));
    fb_buf_2 = malloc(lcddev.width * lcddev.height * sizeof(u_int32_t));
    /* 对图片解码获取RGB888的原始数据 */
    if (0 != png_decode(&img_data, "/p/scr1.png"))
    {
        printf("png_decode failed!!");
    }
    printf("图片长: %d  宽: %d\n", img_data.w, img_data.h);
    int8_t flag = 0;
    for (u_int16_t i = 0; i < 1024; i += 5)
    {
        if (flag == 0)
        {
            flag = 1;
            memset(fb_buf_1, 0xFF, lcddev.screenSize);
            show_image(&lcddev, fb_buf_1, &img_data, i, 100);
            memcpy(lcddev.screenBase, fb_buf_1, lcddev.screenSize);
        }
        else
        {
            flag = 0;
            memset(fb_buf_2, 0xFF, lcddev.screenSize);
            show_image(&lcddev, fb_buf_2, &img_data, i, 100);
            memcpy(lcddev.screenBase, fb_buf_2, lcddev.screenSize);
        }
    }
    /* 释放图片内存 */
    free(img_data.data);
    /* 释放两个全屏显示buf */
    free(fb_buf_1);
    free(fb_buf_2);
    /* 释放显示屏幕资源 */
    lcddev.delete(&lcddev);
}


```