#include "lcd.h"

/********************************************************************
 * 函数名称： lcd_draw_point
 * 功能描述： 打点
 * 输入参数： x, y, color
 * 返 回 值： 无
 ********************************************************************/
void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color); // 得到 RGB565 颜色值
    /* 对传入参数的校验 */
    if (x >= width)
        x = width - 1;
    if (y >= height)
        y = height - 1;
    /* 填充颜色 */
    screen_base[y * width + x] = rgb565_color;
}
/********************************************************************
 * 函数名称： lcd_draw_line
 * 功能描述： 画线（水平或垂直线）
 * 输入参数： x, y, dir, length, color
 * 返 回 值： 无
 ********************************************************************/
void lcd_draw_line(unsigned int x, unsigned int y, int dir,
                   unsigned int length, unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color); // 得到 RGB565 颜色值
    unsigned int end;
    unsigned long temp;
    /* 对传入参数的校验 */
    if (x >= width)
        x = width - 1;
    if (y >= height)
        y = height - 1;
    /* 填充颜色 */
    temp = y * width + x; // 定位到起点
    if (dir)
    { // 水平线
        end = x + length - 1;
        if (end >= width)
            end = width - 1;
        for (; x <= end; x++, temp++)
            screen_base[temp] = rgb565_color;
    }
    else
    { // 垂直线
        end = y + length - 1;
        if (end >= height)
            end = height - 1;
        for (; y <= end; y++, temp += width)
            screen_base[temp] = rgb565_color;
    }
}
/********************************************************************
 * 函数名称： lcd_draw_rectangle
 * 功能描述： 画矩形
 * 输入参数： start_x, end_x, start_y, end_y, color
 * 返 回 值： 无
 ********************************************************************/
void lcd_draw_rectangle(unsigned int start_x, unsigned int end_x,
                        unsigned int start_y, unsigned int end_y,
                        unsigned int color)
{
    int x_len = end_x - start_x + 1;
    int y_len = end_y - start_y - 1;
    lcd_draw_line(start_x, start_y, 1, x_len, color);     // 上边
    lcd_draw_line(start_x, end_y, 1, x_len, color);       // 下边
    lcd_draw_line(start_x, start_y + 1, 0, y_len, color); // 左边
    lcd_draw_line(end_x, start_y + 1, 0, y_len, color);   // 右边
}
/********************************************************************
 * 函数名称： lcd_fill
 * 功能描述： 将一个矩形区域填充为参数 color 所指定的颜色
 * 输入参数： start_x, end_x, start_y, end_y, color
 * 返 回 值： 无
 ********************************************************************/
void lcd_fill(unsigned int start_x, unsigned int end_x,
              unsigned int start_y, unsigned int end_y,
              unsigned int color)
{
    unsigned short rgb565_color = argb8888_to_rgb565(color); // 得到 RGB565 颜色值
    unsigned long temp;
    unsigned int x;
    /* 对传入参数的校验 */
    if (end_x >= width)
        end_x = width - 1;
    if (end_y >= height)
        end_y = height - 1;
    /* 填充颜色 */
    temp = start_y * width; // 定位到起点行首
    for (; start_y <= end_y; start_y++, temp += width)
    {
        for (x = start_x; x <= end_x; x++)
            screen_base[temp + x] = rgb565_color;
    }
}