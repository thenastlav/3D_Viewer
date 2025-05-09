#include "affine.h"

void scale(const float theta, float *result) {
  identity_matrix(result);
  result[0] = theta;
  result[5] = theta;
  result[10] = theta;
  result[15] = 1.0f;
}

void rotate_x(const float angle, float *result) {
  float c = cosf(angle);
  float s = sinf(angle);

  identity_matrix(result);
  result[5] = c;
  result[6] = -s;
  result[9] = s;
  result[10] = c;
}

void rotate_y(const float angle, float *result) {
  float c = cosf(angle);
  float s = sinf(angle);

  identity_matrix(result);
  result[0] = c;
  result[2] = s;
  result[8] = -s;
  result[10] = c;
}

void rotate_z(const float angle, float *result) {
  float c = cosf(angle);
  float s = sinf(angle);

  identity_matrix(result);
  result[0] = c;
  result[1] = -s;
  result[4] = s;
  result[5] = c;
}

void ortho_projection(float *result) {
  static float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f;
  static float near = 1.0f, far = 100.0f;
  identity_matrix(result);

  result[0] = 2.0f / (right - left);
  result[5] = 2.0f / (top - bottom);
  result[10] = -2.0f / (far - near);
  result[12] = -(right + left) / (right - left);
  result[13] = -(top + bottom) / (top - bottom);
  result[14] = -(far + near) / (far - near);
}

void perspective_projection(float *result) {
  static float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f;
  static float near = 1.0f, far = 100.0f;
  identity_matrix(result);

  result[0] = 2.0f * near / (right - left);
  result[5] = 2.0f * near / (top - bottom);
  result[8] = (right + left) / (right - left);
  result[9] = (top + bottom) / (top - bottom);
  result[10] = -(far + near) / (far - near);
  result[11] = -1.0f;
  result[14] = -2.0f * near / (far - near);
  result[15] = 0.0f;
}

void identity_matrix(float *result) {
  for (int i = 0; i < 16; i++) {
    result[i] = i % 5 == 0;
  }
}

float radians(const float angle) { return angle * M_PI / 180.0f; }

void mat4_multiply(const float *a, const float *b, float *result) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result[i * 4 + j] =
          a[i * 4 + 0] * b[0 * 4 + j] + a[i * 4 + 1] * b[1 * 4 + j] +
          a[i * 4 + 2] * b[2 * 4 + j] + a[i * 4 + 3] * b[3 * 4 + j];
    }
  }
}