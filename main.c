#include <stdio.h>
#include "common.h"
#include "draw.h"

static LRESULT CALLBACK win_wndproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static HWND win_create(const char * title, UINT width, UINT height, hwnd_context_t * ctx);
static int win_update(HWND hwnd);
static void win_destroy(HWND hwnd);

hwnd_context_t context = {
	TRUE,
	800, 600,
	NULL,
	{ NULL },
};

int main(int argc, char ** argv) {
	context.bitmap = malloc(800 * 600 * 3);
	if (context.bitmap == NULL) {
		return 1;
	}

	HWND hwnd = win_create("test", context.width, context.height, &context);
	if (hwnd == NULL) {
		return 2;
	}
	draw_bind(&context);

	while (context.running && win_update(hwnd) == 0) {
		Sleep(16);
		draw_clear(RGB(0, 0, 0));
	}

	return 0;
}

static LRESULT CALLBACK win_wndproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC dc = WIN_CALL(BeginPaint(hwnd, &ps));
			GL_CALL(glDrawPixels(context.width, context.height, GL_RGB, GL_UNSIGNED_BYTE, context.bitmap));
			WIN_CALL(SwapBuffers(ps.hdc));
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

	HWND h = WIN_CALL(CreateWindowExA(0, class_name, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, NULL));
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

	HDC dc = GetDC(h);
	int win_choose_pfd = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, win_choose_pfd, &pfd);

	context.gl.ctx = wglCreateContext(dc);
	wglMakeCurrent(dc, context.gl.ctx);
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

static void win_destroy(HWND hwnd) {
	wglDeleteContext(context.gl.ctx);
	DestroyWindow(hwnd);
	UnregisterClassA("krisvers' window class", GetModuleHandle(NULL));
}