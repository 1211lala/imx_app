#pragma once

#define argb8888_to_rgb565(color) ({ \
    unsigned int temp = (color);     \
    ((temp & 0xF80000UL) >> 8) |     \
        ((temp & 0xFC00UL) >> 5) |   \
        ((temp & 0xF8UL) >> 3);      \
})

extern int width;                   // LCD X 分辨率
extern int height;                  // LCD Y 分辨率
extern unsigned short *screen_base; // 映射后的显存基地址

void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color);
void lcd_draw_line(unsigned int x, unsigned int y, int dir, unsigned int length, unsigned int color);
void lcd_fill(unsigned int start_x, unsigned int end_x,
              unsigned int start_y, unsigned int end_y, unsigned int color);
void lcd_draw_rectangle(unsigned int start_x, unsigned int end_x,
                        unsigned int start_y, unsigned int end_y, unsigned int color);

void *lcd_show(void *arg);
int lcd_get_info(void);
int lcd_show_thread_start(void);
