#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "draw.h"
#include "math.h"

static LRESULT CALLBACK win_wndproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static HWND win_create(const char * title, UINT width, UINT height, hwnd_context_t * ctx);
static int win_update(HWND hwnd);
static void win_render(HWND hwnd);
static void win_destroy(HWND hwnd);

static void vert(unsigned long long int id, float x, float y, float* out_x, float* out_y) {
	*out_x = x;
	*out_y = y;
}

static void frag(unsigned long long int v0, unsigned long long int v1, unsigned long long int v2, float x, float y, float * out_r, float * out_g, float * out_b, float * out_a) {
	*out_r = (x + 1) / 2;
	*out_g = (y + 1) / 2;
	*out_b = 0;
	*out_a = 1;
}

hwnd_context_t context = {
	TRUE,
	800, 600,
	NULL,
	{ NULL },
	vert,
	frag,
};

int main(int argc, char ** argv) {
	context.bitmap = malloc(context.width * context.height * 3);
	if (context.bitmap == NULL) {
		return 1;
	}

	HWND hwnd = win_create("test", context.width, context.height, &context);
	if (hwnd == NULL) {
		return 2;
	}
	context.hwnd = hwnd;
	draw_bind(&context);
	draw_set(draw_get() | DRAW_SETTING_FILL);

	float vertices[8] = {
		-2, -2,
		-2, 2,
		2, -2,
		2, 2,
	};

	unsigned long long int indices[6] = {
		0, 1, 2,
		1, 3, 2,
	};

	while (context.running && win_update(hwnd) == 0) {
		draw_clear(KRGB(0x00, 0x00, 0x00));
		draw_indices(vertices, indices, 6);
		win_render(hwnd);
	}

	win_destroy(hwnd);
	free(context.bitmap);

	return 0;
}

static LRESULT CALLBACK win_wndproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc = WIN_CALL(BeginPaint(hwnd, &ps));
			WIN_CALL(EndPaint(hwnd, &ps));
		}
		return 0;
	}

	return DefWindowProcA(hwnd, msg, w, l);
}

static HWND win_create(const char * title, UINT width, UINT height, hwnd_context_t * ctx) {
	const char * class_name = "krisvers' window class";
	WNDCLASSA cls;
	ZeroMemory(&cls, sizeof(cls));
	cls.lpfnWndProc = win_wndproc;
	cls.hInstance = GetModuleHandle(NULL);
	cls.lpszClassName = class_name;
	WIN_CALL(RegisterClassA(&cls));

	RECT r = {
		0, 0,
		width, height
	};

	WIN_CALL(AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW, FALSE, 0));

	HWND h = WIN_CALL(CreateWindowExA(0, class_name, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, NULL, NULL));
	if (h == NULL) {
		return NULL;
	}
	WIN_CALL(ShowWindow(h, SW_SHOW));

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC dc = WIN_CALL(GetDC(h));
	int win_choose_pfd = WIN_CALL(ChoosePixelFormat(dc, &pfd));
	WIN_CALL(SetPixelFormat(dc, win_choose_pfd, &pfd));

	context.gl.ctx = WIN_CALL(wglCreateContext(dc));
	WIN_CALL(wglMakeCurrent(dc, context.gl.ctx));
	glViewport(0, 0, width, height);

	return h;
}

static int win_update(HWND hwnd) {
	MSG msg;
	BOOL b = WIN_CALL(PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE));
	if (b > 0) {
		WIN_CALL(TranslateMessage(&msg));
		DispatchMessageA(&msg);
		if (!IsWindow(hwnd)) {
			return 1;
		}
	}

	return 0;
}

static void win_render(HWND hwnd) {
	GL_CALL(glDrawPixels(context.width, context.height, GL_RGB, GL_UNSIGNED_BYTE, context.bitmap));
	WIN_CALL(SwapBuffers(GetWindowDC(hwnd)));
}

static void win_destroy(HWND hwnd) {
	wglDeleteContext(context.gl.ctx);
	DestroyWindow(hwnd);
	UnregisterClassA("krisvers' window class", GetModuleHandle(NULL));
}