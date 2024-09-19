#include "freetype_fun.h"

FT_Library library;
FT_Face face;

int freetype_init(const char *fontpath, int angle)
{
    FT_Error error;
    FT_Vector pen;
    FT_Matrix matrix;
    float rad;
    /* FreeType 初始化 */
    FT_Init_FreeType(&library);
    /* 加载 face 对象 */
    error = FT_New_Face(library, fontpath, 0, &face);
    if (error)
    {
        fprintf(stderr, "FT_New_Face error: %d\n", error);
        exit(EXIT_FAILURE);
    }
    /* 原点坐标 */
    pen.x = 0 * 64;
    pen.y = 0 * 64; // 原点设置为(0, 0)
    /* 2x2 矩阵初始化 */
    rad = (1.0 * angle / 180) * 3.1415926; // （角度转换为弧度）M_PI 是圆周率
#if 0                                      // 非水平方向
 matrix.xx = (FT_Fixed)( cos(rad) * 0x10000L);
 matrix.xy = (FT_Fixed)(-sin(rad) * 0x10000L);
 matrix.yx = (FT_Fixed)( sin(rad) * 0x10000L);
 matrix.yy = (FT_Fixed)( cos(rad) * 0x10000L);
#endif
#if 1 // 斜体 水平方向显示的
    matrix.xx = (FT_Fixed)(cos(rad) * 0x10000L);
    matrix.xy = (FT_Fixed)(sin(rad) * 0x10000L);
    matrix.yx = (FT_Fixed)(0 * 0x10000L);
    matrix.yy = (FT_Fixed)(1 * 0x10000L);
#endif
    /* 设置 */
    FT_Set_Transform(face, &matrix, &pen);
    FT_Set_Pixel_Sizes(face, 50, 0); // 设置字体大小
    return 0;
}

void lcd_draw_character(struct _lcddev *lcd, int x, int y, const wchar_t *str, unsigned int color)
{
    __uint32_t *addr = lcd->screenBase;

    const int width = 1024;
    const int height = 600;
    // unsigned short rgb565_color = argb8888_to_rgb565(color); // 得到 RGB565 颜色值
    FT_GlyphSlot slot = face->glyph;
    size_t len = wcslen(str); // 计算字符的个数
    long int temp;
    int n;
    int i,       // 在屏幕上显示的坐标X
        j,       // 在屏幕上显示的坐标Y
        p,       // 计算freetype数据开始显示的起始列数
        q;       // 计算freetype数据开始显示的起始行数
    int max_x,   // 字体单位宽度
        max_y,   // 字体单位高度
        start_y, // 字体开始显示的Y坐标
        start_x; // 字体开始显示的X坐标

    // 循环加载各个字符
    for (n = 0; n < len; n++)
    {
        // 加载字形、转换得到位图数据
        if (FT_Load_Char(face, str[n], FT_LOAD_RENDER))
            continue;
        /* start_y 为显示的左上角点的y  */
        start_y = y - slot->bitmap_top; // 计算字形轮廓上边 y 坐标起点位置 注意是减去 bitmap_top
        /* 字的显示高度大于当前的y值 */
        if (0 > start_y)
        { // 如果为负数 如何处理？？
            q = -start_y;
            temp = 0;
            j = 0;
        }
        else
        { // 正数又该如何处理??
            q = 0;
            temp = width * start_y;
            j = start_y;
        }

        /* 计算最大的y坐标值  如果超过屏幕等于屏幕最大y值 */
        max_y = start_y + slot->bitmap.rows; // 计算字形轮廓下边 y 坐标结束位置
        if (max_y > (int)height)
            max_y = height;

        for (; j < max_y; j++, q++, temp += width)
        {
            start_x = x + slot->bitmap_left; // 起点位置要加上左边空余部分长度
            if (0 > start_x)
            {
                p = -start_x;
                i = 0;
            }
            else
            {
                p = 0;
                i = start_x;
            }
            max_x = start_x + slot->bitmap.width;
            if (max_x > (int)width)
                max_x = width;
            for (; i < max_x; i++, p++)
            {
                // 如果数据不为 0，则表示需要填充颜色
                if (slot->bitmap.buffer[q * slot->bitmap.width + p])
                    addr[temp + i] = color;
            }
        }
        // 调整到下一个字形的原点
        x += slot->advance.x / 64; // 26.6 固定浮点格式
        y -= slot->advance.y / 64;
    }
}