CC=g++
FLAGS=-Wall -Werror -Wextra -std=c++17
LIBS=-lgtest -lgtest_main
TARGET=target
LEAKS=leaks --atExit -- $(TARGET)/tests

OS=$(shell uname -s)
ifeq ($(OS), Linux)
	LIBS+=-lpthread
	LEAKS=valgrind $(TARGET)/tests
endif

all: install tests

install:

uninstall:
	@rm -rf build/*.app

clean:
	@rm -Rf *.o *.gcno *.gcda *.info $(TARGET) .clang-format maze_tar

dvi:

dist: clean
	@mkdir maze_tar
	@tar -cf maze_tar/Maze_src.tgz Makefile Controller Model Test View

tests: test_build
	@$(TARGET)/tests

clang:
	@cp ../materials/linters/.clang-format .
	@clang-format -i Model/*.h Model/include/*.h Model/impl/*.h Test/*.cc
	@clang-format -i Controller/*.h
	@clang-format -i View/*.h
	@clang-format -n Model/*.h Model/include/*.h Model/impl/*.h Test/*.cc Test/*.cc
	@clang-format -n Controller/*.h 
	@clang-format -n View/*.h

check:
	@cppcheck --language=c++ Model/*.h Model/include/*.h Model/impl/*.h Test/*.cc
	@echo "Model and Test checked!"
	@cppcheck --language=c++ Controller/*.h
	@echo "Controller checked!"
	@cppcheck --language=c++ View/*.h
	@echo "View checked!"

leaks: test_build
	$(LEAKS)

test_build: clang clean
	@mkdir $(TARGET)
	@touch $(TARGET)/save_file.txt
	@$(CC) $(FLAGS) Test/tests.cc -o $(TARGET)/tests $(LIBS)

git: clean
	git add .
	git commit -m "add dist"
	git push origin birdpers
