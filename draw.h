#ifndef KRISVERS_RASTERIZER_DRAW_H
#define KRISVERS_RASTERIZER_DRAW_H

#include "common.h"

#define KRGB(r, g, b) r, g, b, 0xFF
#define KRGBA(r, g, b, a) r, g, b, a
#define KBGR(b, g, r) r, g, b, 0xFF
#define KBGRA(b, g, r, a) r, g, b, a
#define KRGB_UINT(col) (col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, 0xFF
#define KRGBA_UINT(col) (col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, col & 0xFF
#define KBGR_UINT(col) (col >> 8) & 0xFF, (col >> 16) & 0xFF, (col >> 24) & 0xFF, 0xFF
#define KBGRA_UINT(col) (col >> 8) & 0xFF, (col >> 16) & 0xFF, (col >> 24) & 0xFF, col & 0xFF

#define DRAW_SETTING_FILL 0
#define DRAW_SETTING_WIREFRAME 1
#define DRAW_SETTING_POINT 2
#define DRAW_SETTING_DEFAULT (DRAW_SETTING_FILL)

void draw_bind(hwnd_context_t * ctx);
void draw_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_pixel_clip(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_line(float x0, float y0, float x1, float y1, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void draw_vertices(float * vertices, unsigned long long int length);
void draw_indices(float * vertices, unsigned long long int * indices, unsigned long long int length);
void draw_set(unsigned int setting);
unsigned int draw_get(void);

#endif