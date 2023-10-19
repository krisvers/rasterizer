#include <windows.h>
#include <stdio.h>

#if _DEBUG
#define WIN_CALL(statement) statement; { DWORD err = GetLastError(); if (err != 0) { printf("WIN32 Error (%s:%u): %i 0x%x\n", __FILE__, __LINE__, err, err); } }
#else
#define WIN_CALL(statement) statement;
#endif

typedef struct {
	BOOL running;
} hwnd_context_t;

static LRESULT CALLBACK win_wndproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
static HWND win_create(const char * title, UINT width, UINT height);
static int win_update(HWND hwnd);
static void win_destroy(HWND hwnd);

int main(int argc, char ** argv) {
	hwnd_context_t ctx = {
		TRUE
	};

	HWND hwnd = win_create("test", 800, 600, &ctx);
	if (hwnd == NULL) {
		return 1;
	}

	while (ctx.running && win_update(hwnd) == 0) {
	}

	return 0;
}

static LRESULT CALLBACK win_wndproc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
	switch (msg) {
	case WM_DESTROY:
		WIN_CALL(PostQuitMessage(0));
		hwnd_context_t * ctx = GetWindowLongPtrA(hwnd, -21);
		ctx->running = FALSE;
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = WIN_CALL(BeginPaint(hwnd, &ps));
			WIN_CALL(FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1)));
			WIN_CALL(EndPaint(hwnd, &ps));
		}
		return 0;
	}

	return DefWindowProcA(hwnd, msg, w, l);
}

static HWND win_create(const char * title, UINT width, UINT height, hwnd_context_t * ctx) {
	const char * class_name = "krisvers' window class";
	WNDCLASS cls;
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
	WIN_CALL(UpdateWindow(h));
	WIN_CALL(SetWindowLongPtrA(h, -21, ctx));

	return h;
}

static int win_update(HWND hwnd) {
	MSG msg;
	BOOL b = WIN_CALL(PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE));
	if (b != 0) {
		WIN_CALL(TranslateMessage(&msg));
		DispatchMessageA(&msg);
		if (!IsWindow(hwnd)) {
			return 1;
		}
	}

	return 0;
}

static void win_destroy(HWND hwnd) {

}