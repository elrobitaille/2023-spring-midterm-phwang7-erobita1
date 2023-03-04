# Midterm Project
# Edgar Robitaille - erobita1
# <your name> and <your JHED>
GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

LDLIBS = -lm

all: puzzle

puzzle: ppm_io.o puzzle_funcs.o puzzle.o
	$(GCC) -o puzzle ppm_io.o puzzle_funcs.o puzzle.o $(LDLIBS)

ppm_io.o: ppm_io.c ppm_io.h
	$(GCC) -c ppm_io.c $(CFLAGS)

puzzle_funcs.o: puzzle_funcs.c ppm_io.h puzzle.h
	$(GCC) -c puzzle_funcs.c $(CFLAGS)

puzzle.o: puzzle.c puzzle.h ppm_io.h
	$(GCC) -c puzzle.c $(CFLAGS)

clean:
	rm -f puzzle *.o

run: puzzle
	./puzzle $(input_file)