#pragma once

#include "datafile.h"

int show_png_image(struct _lcddev *lcd, const char *path, u_int16_t x, u_int16_t y);
int png_decode(struct IMG_DATA *png_data, const char *path);