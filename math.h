#ifndef KRISVERS_RASTERIZER_MATH_H
#define KRISVERS_RASTERIZER_MATH_H

float math_lerp(float v0, float v1, float t);
void math_bilerp(float x0, float y0, float x1, float y1, float t, float * out_x, float * out_y);

#endif