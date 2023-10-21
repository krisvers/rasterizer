#ifndef KRISVERS_RASTERIZER_COMMON_H
#define KRISVERS_RASTERIZER_COMMON_H

#include <windows.h>
#include <GL/GL.h>

typedef struct {
	HGLRC ctx;
} gl_context_t;

typedef struct {
	BOOL running;
	UINT width;
	UINT height;
	unsigned char* bitmap;
	gl_context_t gl;
} hwnd_context_t;

#if _DEBUG
#define WIN_CALL(statement) statement; { DWORD err = GetLastError(); if (err != 0) { printf("WIN32 Error (%s:%u): %i 0x%x\n", __FILE__, __LINE__, err, err); } }
#define GL_CALL(statement) statement; { int err = glGetError(); if (err != 0) { printf("GL Error (%s:%u): %i 0x%x\n", __FILE__, __LINE__, err, err); } }
#else
#define WIN_CALL(statement) statement;
#define GL_CALL(statement) statement;
#endif

#endif