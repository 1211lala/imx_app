#pragma once

struct PNG_DATA
{
    u_int16_t width;
    u_int16_t height;
    u_int32_t *data;
};
extern struct PNG_DATA png_data;

int show_png_image(struct _lcddev *lcd, const char *path, u_int16_t x, u_int16_t y);
int png_decode(struct PNG_DATA *png_data, const char *path);