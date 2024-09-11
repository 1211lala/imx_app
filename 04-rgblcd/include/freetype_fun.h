#pragma once
#include <wchar.h>
#include <math.h>
#include "datafile.h"
#include "ft2build.h"
#include FT_FREETYPE_H
extern FT_Library library;
extern FT_Face face;

int freetype_init(const char *font, int angle);
void lcd_draw_character(struct _lcddev *lcd, int x, int y, const wchar_t *str, unsigned int color);


