#include "draw.h"
#include <stdio.h>

static hwnd_context_t * context = NULL;

void draw_bind(hwnd_context_t * ctx) {
	context = ctx;
}

void draw_pixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (context == NULL || x >= context->width || y >= context->height || a == 0) {
		return;
	}

	context->bitmap[(x + (y * context->width)) * 3] = r;
	context->bitmap[(x + (y * context->width)) * 3 + 1] = g;
	context->bitmap[(x + (y * context->width)) * 3 + 2] = b;
}

void draw_clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	if (a == 0) {
		return;
	}

	for (unsigned int h = 0; h < context->height; ++h) {
		for (unsigned int w = 0; w < context->width; ++w) {
			context->bitmap[(w + (h * context->width)) * 3] = r;
			context->bitmap[(w + (h * context->width)) * 3 + 1] = g;
			context->bitmap[(w + (h * context->width)) * 3 + 2] = b;
		}
	}
}

void draw_pixel_clip(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	unsigned int ux = (x + 1) * (context->width / 2.0f);
	unsigned int uy = (y + 1) * (context->height / 2.0f);
	printf("(%f + 1) * %u / 2 = %u\n(%f + 1) * %u / 2 = %u\n", x, context->width, ux, y, context->height, uy);
	draw_pixel(ux, uy, r, g, b, a);
}

void draw_line(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	float dx = x2 - x1;
	float dy = y2 - y1;

	float w = context->width;
	float h = context->height;
}