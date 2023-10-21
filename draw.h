#ifndef KRISVERS_RASTERIZER_DRAW_H
#define KRISVERS_RASTERIZER_DRAW_H

#include "common.h"

#define RGB(r, g, b) r, g, b, 0xFF
#define RGBA(r, g, b, a) r, g, b, a
#define BGR(b, g, r) r, g, b, 0xFF
#define BGRA(b, g, r, a) r, g, b, a
#define RGB_UINT(col) (col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, 0xFF
#define RGBA_UINT(col) (col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF
#define BGR_UINT(col) (col >> 8) & 0xFF, (col >> 16) & 0xFF, (col >> 24) & 0xFF, 0xFF
#define BGRA_UINT(col) (col >> 8) & 0xFF, (col >> 16) & 0xFF, (col >> 24) & 0xFF, col & 0xFF

void draw_bind(hwnd_context_t * ctx);
void draw_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_pixel_clip(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_line(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

#endif