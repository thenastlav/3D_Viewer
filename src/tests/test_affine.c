#include "test_viewer.h"
#include "../model/affine.h"

int matrix_equals(const float* a, const float* b, float epsilon) {
  for (int i = 0; i < 16; i++) {
    if (fabs(a[i] - b[i]) > epsilon) {
      return 0; 
    }
  }
  return 1; 
}

START_TEST(test_scale) {
  float result[16];
  float expected[16] = {
      2.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 2.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  scale(2.0f, result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

START_TEST(test_rotate_x) {
  float result[16];
  float angle = radians(90.0f);
  float expected[16] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  rotate_x(angle, result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

START_TEST(test_rotate_y) {
  float result[16];
  float angle = radians(90.0f);
  float expected[16] = {
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  rotate_y(angle, result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

START_TEST(test_rotate_z) {
  float result[16];
  float angle = radians(90.0f); 
  float expected[16] = {
      0.0f, -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  rotate_z(angle, result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

START_TEST(test_ortho_projection) {
  float result[16];
  float expected[16] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -0.0202f, 0.0f,
      0.0f, 0.0f, -1.0202f, 1.0f
  };

  ortho_projection(result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

START_TEST(test_perspective_projection) {
  float result[16];
  float expected[16] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, -1.0202f, -1.0f,
      0.0f, 0.0f, -0.0202f, 0.0f
  };

  perspective_projection(result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

START_TEST(test_mat4x4_multiply) {
  float a[16] = {
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
  };
  float b[16] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };
  float result[16];
  float expected[16] = {
      1.0f, 2.0f, 3.0f, 4.0f,
      5.0f, 6.0f, 7.0f, 8.0f,
      9.0f, 10.0f, 11.0f, 12.0f,
      13.0f, 14.0f, 15.0f, 16.0f
  };

  mat4_multiply(a, b, result);
  ck_assert(matrix_equals(result, expected, 0.0001f));
}
END_TEST

Suite* matrix_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("Matrix");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_scale);
  tcase_add_test(tc_core, test_rotate_x);
  tcase_add_test(tc_core, test_rotate_y);
  tcase_add_test(tc_core, test_rotate_z);
  tcase_add_test(tc_core, test_ortho_projection);
  tcase_add_test(tc_core, test_perspective_projection);
  tcase_add_test(tc_core, test_mat4x4_multiply);
  suite_add_tcase(s, tc_core);

  return s;
}

Suite *affine_suite() {
  Suite *s;
  s = suite_create("\033[33m3d_affine_tests\033[0m");

  TCase *tc_core;
  tc_core = tcase_create("affine_core");
  tcase_add_test(tc_core, test_scale);
  tcase_add_test(tc_core, test_rotate_x);
  tcase_add_test(tc_core, test_rotate_y);
  tcase_add_test(tc_core, test_rotate_z);
  tcase_add_test(tc_core, test_ortho_projection);
  tcase_add_test(tc_core, test_perspective_projection);
  tcase_add_test(tc_core, test_mat4x4_multiply);
  suite_add_tcase(s, tc_core);

  return s;
}
