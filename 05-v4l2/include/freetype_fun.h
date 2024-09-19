#pragma once
#include <wchar.h>
#include <math.h>
#include "ft2build.h"
#include "datafile.h"

#include FT_FREETYPE_H
extern FT_Library library;
extern FT_Face face;

int freetype_init(const char *font, int angle);
void lcd_draw_character(struct _lcddev *lcd, int x, int y, const wchar_t *str, unsigned int color);
void lcd_draw_character2(u_int16_t *bufaddr, int x, int y, const wchar_t *str, unsigned int color);

