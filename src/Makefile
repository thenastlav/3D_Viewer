CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c11
LIBFLAGS = -lcheck -lm

APP_BIN := viewer
TEST_BIN := test.out

BUILD_DIR := ./build
REPORT_DIR := ./report

SRC_FILES := ./model/*.c ./view/*.cpp
HEADER_FILES := ./model/*.h ./view/*.h ./3rdparty/*.h
VALGRIND_LOG := valgrind-out.txt

ifeq ($(shell uname -s), Linux)
LIBFLAGS += -lsubunit
endif

.PHONY: all install uninstall test gcov_report dvi clean check clang

all: install
	./$(BUILD_DIR)/$(APP_BIN) > /dev/null 2>&1

install: clean
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && qmake ../viewer.pro && make
	@echo "viewer has been installed successfully."

uninstall:
	@rm -rf $(BUILD_DIR)
	@echo "viewer has been successfully removed."

test:
	@rm -f *.gcda *.gcno
	@$(CC) $(CFLAGS) --coverage ./tests/*.c ./model/*.c $(LIBFLAGS) -o $(TEST_BIN)
	./$(TEST_BIN)
	
gcov_report: test
	@lcov -t "test" -o test.info -c -d .
	@genhtml -o $(REPORT_DIR) test.info
	open ./$(REPORT_DIR)/index.html

dvi:
	open html/index.html

dist: install 
	cd .. && tar -czf $(APP_BIN).tgz src

clean: uninstall
	@rm -rf $(APP_BIN) $(TEST_BIN) $(VALGRIND_LOG) $(REPORT_DIR) *.o *.gcda *.gcno *.info *.dSYM ../$(APP_BIN).tgz

check:
	@$(CC) $(CFLAGS) ./tests/*.c ./model/*.c $(LIBFLAGS) -o $(TEST_BIN)
	@valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=$(VALGRIND_LOG) \
         ./$(TEST_BIN)
	@tail -n8 $(VALGRIND_LOG)

clang:
	clang-format -n main.cpp $(SRC_FILES) $(HEADER_FILES)
	clang-format -i main.cpp $(SRC_FILES) $(HEADER_FILES)