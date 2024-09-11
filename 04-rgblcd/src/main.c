#include "main.h"
#include "datafile.h"
#include "jpeg_fun.h"
#include "png_fun.h"
#include "freetype_fun.h"

struct _lcddev lcddev = {
    .init = rgblcd_init,
    .drawPonit = lcd_draw_point,
    .showJpg = show_jpeg_image,
    .showPng = show_png_image,
    .showImage = show_image,
    .delete = lcd_delete,
};

int main(int argc, char *argv[])
{
    if (0 != lcddev.init(&lcddev))
    {
        printf("rgblcd_init failed!!");
    }
    memset(lcddev.screenBase, 0xff, lcddev.screenSize);
    freetype_init(argv[1], 0);

    while (1)
    {
        for (u_int8_t i = 10; i < 100; i += 10)
        {
            memset(lcddev.screenBase, 0xff, lcddev.screenSize);
            FT_Set_Pixel_Sizes(face, i, 0); // 设置字体大小
            int y = 600 * 0.25;
            lcd_draw_character(&lcddev, 50, 100, L"路漫漫其修远兮，吾将上下而求索", 0xff0000);
            lcd_draw_character(&lcddev, 50, y + 100, L"莫愁前路无知己，天下谁人不识君", 0x9900FF);
            lcd_draw_character(&lcddev, 50, 2 * y + 100, L"君不见黄河之水天上来，奔流到海不复回", 0xFF0099);
            // lcd_draw_character(&lcddev, 50, 3 * y + 100, L"君不见高堂明镜悲白发，朝如青丝暮成雪", 0x9932CC);
            // 使用宽字符版本的 sprintf 来生成 wchar_t 字符串
            wchar_t wbuf[100];
            swprintf(wbuf, sizeof(wbuf) / sizeof(wchar_t), L"buf : %d", i); // 使用宽字符格式化字符串

            // 传递 wchar_t 字符串给 lcd_draw_character
            lcd_draw_character(&lcddev, 50, 3 * y + 100, wbuf, 0x9932CC);
            sleep(1);
        }
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    lcddev.delete(&lcddev);
}
