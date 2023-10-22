#include "math.h"

float math_lerp(float v0, float v1, float t) {
	return (1 - t) * v0 + t * v1;
}

void math_bilerp(float x0, float y0, float x1, float y1, float t, float * out_x, float * out_y) {
	*out_x = math_lerp(x0, x1, t);
	*out_y = math_lerp(y0, y1, t);
}