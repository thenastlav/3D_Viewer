#include "test_viewer.h"

#include "../model/parser.h"
START_TEST(test_filename_ext) {
  const char* filename1 = "example.obj";

  ck_assert_str_eq(get_filename_ext(filename1), "obj");
  const char* filename2 = "example";

  ck_assert_str_eq(get_filename_ext(filename2), "");
  const char* filename3 = ".hiddenfile";
  ck_assert_str_eq(get_filename_ext(filename3), "");

  const char* filename4 = "archive.tar.gz";
  ck_assert_str_eq(get_filename_ext(filename4), "gz");
}
END_TEST

START_TEST(test_is_obj_file) {
  const char* filename1 = "example.obj";
  ck_assert(is_obj_file(filename1));

  const char* filename2 = "example.txt";
  ck_assert(!is_obj_file(filename2));

  const char* filename3 = "example";
  ck_assert(!is_obj_file(filename3));
}
END_TEST

START_TEST(test_count_vertices_faces) {
  const char* content = "v 1.0 2.0 3.0\nv 4.0 5.0 6.0\nf 1 2 3\nf 4 5 6\n";
  FILE* file = tmpfile();
  fputs(content, file);
  rewind(file);

  size_t n_v = 0, n_f = 0;
  count_vertices_faces(file, &n_v, &n_f);

  ck_assert_uint_eq(n_v, 2);
  ck_assert_uint_eq(n_f, 2);

  fclose(file);
}
END_TEST

START_TEST(test_get_filecontent) {
  const char* content = "Hello, World!";

  char filename[256];
  sprintf(filename, "/tmp/tempfile_%d", rand());

  FILE* new_file = fopen(filename, "w");
  fputs(content, new_file);
  fclose(new_file);

  char* file_content = get_filecontent(filename);
  ck_assert(file_content != NULL);
  ck_assert_str_eq(file_content, content);

  free(file_content);
  remove(filename);
}
END_TEST

Suite *parser_suite() {
  Suite *s;
  s = suite_create("\033[33m3d_parser_tests\033[0m");

  TCase *tc_core;
  tc_core = tcase_create("parser_core");
  tcase_add_test(tc_core, test_filename_ext);
  tcase_add_test(tc_core, test_is_obj_file);
  tcase_add_test(tc_core, test_count_vertices_faces);
  tcase_add_test(tc_core, test_get_filecontent);
  suite_add_tcase(s, tc_core);

  return s;
}
