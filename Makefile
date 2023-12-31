# Midterm Project
# Edgar Robitaille erobita1
# Patrick Hwang phwang7
GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

LDLIBS = -lm

all: puzzle puzzle_tests

puzzle: ppm_io.o puzzle_funcs.o puzzle.o
	$(GCC) -o puzzle ppm_io.o puzzle_funcs.o puzzle.o $(LDLIBS)

ppm_io.o: ppm_io.c ppm_io.h
	$(GCC) -c ppm_io.c $(CFLAGS)

puzzle_funcs.o: puzzle_funcs.c ppm_io.h puzzle.h
	$(GCC) -c puzzle_funcs.c $(CFLAGS)

puzzle.o: puzzle.c puzzle.h ppm_io.h 
	$(GCC) -c puzzle.c $(CFLAGS)

puzzle_tests: puzzle_tests.o puzzle_funcs.o ppm_io.o
	$(GCC) -o puzzle_tests puzzle_funcs.o puzzle_tests.o ppm_io.o $(LDLIBS)

puzzle_tests.o: puzzle_tests.c puzzle.h
	$(GCC) -c puzzle_tests.c $(CFLAGS)

clean:
	rm -f puzzle puzzle_tests *.o

test: puzzle_tests
	./puzzle_tests

run: puzzle
	./puzzle $(input_file)