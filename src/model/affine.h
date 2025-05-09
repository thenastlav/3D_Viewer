#ifndef _3D_AFFINE_H
#define _3D_AFFINE_H

#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

#include "object.h"
#define MIN_SCALE 0.1f
#define MAX_SCALE 30.0f

void scale(const float theta, float *result);

void rotate_x(const float delta, float *result);
void rotate_y(const float delta, float *result);
void rotate_z(const float delta, float *result);

void ortho_projection(float *result);
void perspective_projection(float *result);

float radians(const float angle);
void identity_matrix(float *result);
void mat4_multiply(const float *a, const float *b, float *result);

#endif  // _3D_AFFINE_H