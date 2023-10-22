#ifndef KRISVERS_RASTERIZER_COMMON_H
#define KRISVERS_RASTERIZER_COMMON_H

#include <windows.h>
#include <GL/GL.h>

typedef struct {
	HGLRC ctx;
} gl_context_t;

typedef void (*vertex_shader_callback)(unsigned long long int id, float x, float y, float* out_x, float* out_y);
typedef void (*fragment_shader_callback)(unsigned long long int v0, unsigned long long int v1, unsigned long long int v2, float x, float y, float* out_r, float* out_g, float* out_b, float* out_a);

typedef struct {
	BOOL running;
	UINT width;
	UINT height;
	unsigned char* bitmap;
	gl_context_t gl;
	vertex_shader_callback vert_callback;
	fragment_shader_callback frag_callback;
	HWND hwnd;
} hwnd_context_t;

#if _DEBUG
#define WIN_CALL(statement) statement; { DWORD err = GetLastError(); if (err != 0) { printf("WIN32 Error (%s:%u): %i 0x%x\n", __FILE__, __LINE__, err, err); } }
#define GL_CALL(statement) statement; { int err = glGetError(); if (err != 0) { printf("GL Error (%s:%u): %i 0x%x\n", __FILE__, __LINE__, err, err); } }
#else
#define WIN_CALL(statement) statement;
#define GL_CALL(statement) statement;
#endif

#endif