#include <check.h>

#include "test_viewer.h"

int main(void) {
  int failed = 0;
  Suite* tests[] = {affine_suite(), object_suite(), parser_suite(), NULL};

  for (int i = 0; tests[i] != NULL; i++) {
    SRunner* sr = srunner_create(tests[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }

  printf("\n=========== \033[31mFAILED\033[0m: %d =============\n\n", failed);
  return failed;
}