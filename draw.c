#include <stdio.h>
#include "draw.h"
#include "math.h"

static hwnd_context_t * context = NULL;
static unsigned int settings = 0;

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

static void cstoss(float x, float y, unsigned int* out_x, unsigned int* out_y) {
	if (x == 1) {
		*out_x = context->width - 1;
	}
	else {
		*out_x = (unsigned int) ((x + 1) * (context->width / 2.0f));
	}
	if (y == 1) {
		*out_y = context->height - 1;
	}
	else {
		*out_y = (unsigned int) ((y + 1) * (context->height / 2.0f));
	}
}

static void sstocs(unsigned int x, unsigned int y, float * out_x, float * out_y) {
	if (x == context->width - 1) {
		*out_x = 1;
	}
	else {
		*out_x = ((float) x / context->width) * 2 - 1;
	}
	if (y == context->height - 1) {
		*out_y = 1;
	}
	else {
		*out_y = ((float) y / context->height) * 2 - 1;
	}
}

void draw_pixel_clip(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	unsigned int ux = 0;
	unsigned int uy = 0;
	cstoss(x, y, &ux, &uy);
	draw_pixel(ux, uy, r, g, b, a);
}

void draw_line(float x0, float y0, float x1, float y1, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	unsigned int sy0 = 0;
	unsigned int sx0 = 0;
	cstoss(x0, y0, &sx0, &sy0);
	unsigned int sy1 = 0;
	unsigned int sx1 = 0;
	cstoss(x1, y1, &sx1, &sy1);
	int dx = sx1 - sx0;
	int dy = sy1 - sy0;
	int sdx = 1;
	int sdy = 1;
	if (dx < 0) {
		sdx = -1;
		dx = -dx;
	}
	if (dy < 0) {
		sdy = -1;
	} else {
		dy = -dy;
	}
	int error = dx + dy;

	while (1) {
		if (sx0 >= context->width && sy0 >= context->height) {
			break;
		}
		draw_pixel(sx0, sy0, r, g, b, a);
		if (sx0 == sx1 && sy0 == sy1) {
			break;
		}
		int e2 = 2 * error;
		if (e2 >= dy) {
			if (sx0 == sx1) {
				break;
			}
			error = error + dy;
			sx0 = sx0 + sdx;
		}
		if (e2 <= dx) {
			if (sy0 == sy1) {
				break;
			}
			error = error + dx;
			sy0 = sy0 + sdy;
		}
	}
}

void draw_vertices(float * vertices, unsigned long long int length) {
	for (unsigned long long int i = 0; i < length; i += 6) {
		draw_line(vertices[i], vertices[i + 1], vertices[i + 2], vertices[i + 3], KRGB(0xFF, 0x00, 0x00));
		draw_line(vertices[i + 2], vertices[i + 3], vertices[i + 4], vertices[i + 5], KRGB(0x00, 0xFF, 0x00));
		draw_line(vertices[i + 4], vertices[i + 5], vertices[i], vertices[i + 1], KRGB(0x00, 0x00, 0xFF));
	}
}

static void draw_pixel_shader(unsigned long long int v0, unsigned long long int v1, unsigned long long int v2, float x, float y) {
	float r, g, b, a;
	context->frag_callback(v0, v1, v2, x, y, &r, &g, &b, &a);
	r = (r < 0) ? 0 : r;
	g = (g < 0) ? 0 : g;
	b = (b < 0) ? 0 : b;
	a = (a < 0) ? 0 : a;
	draw_pixel_clip(x, y, r * 0xFF, g * 0xFF, b * 0xFF, a * 0xFF);
}

static void draw_line_shader(unsigned long long int v0, unsigned long long int v1, unsigned long long int v2, float x0, float y0, float x1, float y1) {
	unsigned int sy0 = 0;
	unsigned int sx0 = 0;
	cstoss(x0, y0, &sx0, &sy0);
	unsigned int sy1 = 0;
	unsigned int sx1 = 0;
	cstoss(x1, y1, &sx1, &sy1);
	int dx = sx1 - sx0;
	int dy = sy1 - sy0;
	int sdx = 1;
	int sdy = 1;
	if (dx < 0) {
		sdx = -1;
		dx = -dx;
	}
	if (dy < 0) {
		sdy = -1;
	}
	else {
		dy = -dy;
	}
	int error = dx + dy;

	float r, g, b, a;
	float x, y;
	while (1) {
		if (sx0 >= context->width && sy0 >= context->height) {
			break;
		}
		sstocs(sx0, sy0, &x, &y);
		context->frag_callback(v0, v1, v2, x, y, &r, &g, &b, &a);
		r = (r < 0) ? 0 : r;
		g = (g < 0) ? 0 : g;
		b = (b < 0) ? 0 : b;
		a = (a < 0) ? 0 : a;
		draw_pixel(sx0, sy0, r * 0xFF, g * 0xFF, b * 0xFF, a * 0xFF);
		if (sx0 == sx1 && sy0 == sy1) {
			break;
		}
		int e2 = 2 * error;
		if (e2 >= dy) {
			if (sx0 == sx1) {
				break;
			}
			error = error + dy;
			sx0 = sx0 + sdx;
		}
		if (e2 <= dx) {
			if (sy0 == sy1) {
				break;
			}
			error = error + dx;
			sy0 = sy0 + sdy;
		}
	}
}

void draw_indices(float * vertices, unsigned long long int * indices, unsigned long long int length) {
	unsigned long long int i0 = 0;
	unsigned long long int i1 = 0;
	unsigned long long int i2 = 0;
	float v0x = 0;
	float v1x = 0;
	float v2x = 0;
	float v0y = 0;
	float v1y = 0;
	float v2y = 0;

	for (unsigned long long int i = 0; i < length; i += 3) {
		i0 = indices[i];
		i1 = indices[i + 1];
		i2 = indices[i + 2];
		v0x = vertices[i0 * 2];
		v1x = vertices[i1 * 2];
		v2x = vertices[i2 * 2];
		v0y = vertices[i0 * 2 + 1];
		v1y = vertices[i1 * 2 + 1];
		v2y = vertices[i2 * 2 + 1];
		if (settings & DRAW_SETTING_WIREFRAME) {
			draw_line_shader(i0, i1, i2, v0x, v0y, v1x, v1y);
			draw_line_shader(i0, i1, i2, v1x, v1y, v2x, v2y);
			draw_line_shader(i0, i1, i2, v2x, v2y, v0x, v0y);
		}
		if (settings & DRAW_SETTING_POINT) {
			draw_pixel_shader(i0, i1, i2, v0x, v0y);
			draw_pixel_shader(i0, i1, i2, v1x, v1y);
			draw_pixel_shader(i0, i1, i2, v2x, v2y);
		}
		if ((~settings & (DRAW_SETTING_WIREFRAME | DRAW_SETTING_POINT)) == (DRAW_SETTING_WIREFRAME | DRAW_SETTING_POINT)) {
			for (float t = 0; t < 1; t += 0.001f) {
				float x0, y0, x1, y1;
				math_bilerp(v0x, v0y, v1x, v1y, t, &x0, &y0);
				math_bilerp(v2x, v2y, v1x, v1y, t, &x1, &y1);
				draw_line_shader(i0, i1, i2, x0, y0, x1, y1);
			}
		}
	}
}

void draw_set(unsigned int setting) {
	settings = setting;
}

unsigned int draw_get(void) {
	return settings;
}