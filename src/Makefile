CFLAGS = -Wall -Wextra -Werror -std=c11

SOURCES = $(wildcard $(SOURCES_DIR)/*.c)
HEADERS = s21_matrix.h
LIB = s21_matrix.a
OBJECTS = $(SOURCES:.c=.o)

SOURCES_DIR = main_sources
REPORT_DIR = report
TESTS_DIR = tests

OS = $(shell uname)
ifeq ($(OS), Linux)
	TFLAGS = -lcheck -lm -lsubunit
else
	TFLAGS = -lcheck
endif

all: clean $(LIB)

$(LIB): $(OBJECTS)
	ar rcs $(LIB) $(OBJECTS)

clean:
	rm -rf $(SOURCES_DIR)/*.o *.a ./$(TESTS_DIR)/test_exec ./$(TESTS_DIR)/*.gcda ./$(TESTS_DIR)/*.gcno $(REPORT_DIR)
	rm -rf $(SOURCES_DIR)/*.o *.a ./$(TESTS_DIR)/test_exec ./$(TESTS_DIR)/*.gcda ./$(TESTS_DIR)/*.gcno $(REPORT_DIR)
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info

test: $(LIB)
	gcc $(CFLAGS) ./$(TESTS_DIR)/test.c $(LIB) -o ./$(TESTS_DIR)/test_exec $(TFLAGS)
	./$(TESTS_DIR)/test_exec

t: $(LIB)
	gcc $(CFLAGS) ./$(SOURCES_DIR)/test.c $(LIB) -o ./$(TESTS_DIR)/test_exec $(TFLAGS)
	./$(TESTS_DIR)/test_exec

gcov_s21_matrix.a:
	$(CC) $(CFLAGS) -c $(SOURCES_DIR)/s21_*.c -fprofile-arcs -ftest-coverage
	ar rc $(LIB) *.o
	rm -rf *.o

gcov_report: gcov_s21_matrix.a
	mkdir -p $(REPORT_DIR)
	gcc -fprofile-arcs -ftest-coverage ./$(TESTS_DIR)/test.c $(LIB) -o ./$(TESTS_DIR)/test_exec $(TFLAGS)
	./$(TESTS_DIR)/test_exec
#	gcovr --html-details --gcov-exclude=./$(TESTS_DIR)/test_exec -o $(REPORT_DIR)/report.html
	lcov -t ./$(TESTS_DIR)/test_exec -o $(REPORT_DIR)/gcov_report.info -c -d .
	genhtml -o report $(REPORT_DIR)/gcov_report.info
	open $(REPORT_DIR)/index.html

valgrind: test
	2>&1 valgrind --tool=memcheck --leak-check=yes $(TESTS_DIR)/test_exec | grep ERROR

format:
	clang-format -i --style=google $(SOURCES) s21_matrix.h $(TESTS_DIR)/test.c
#	clang-format -n --style=google $(SOURCES) s21_matrix.h $(TESTS_DIR)/test.c
