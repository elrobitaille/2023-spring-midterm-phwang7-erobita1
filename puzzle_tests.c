// Edgar Robitaille erobita1
// Patrick Hwang phwang7
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "puzzle.h"

// also tests puzzle_destroy
void test_puzzle_create(void) {

  Puzzle *p3 = puzzle_create(3);
  Puzzle *p5 = puzzle_create(5);

  assert(p3->size == 3);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      assert(p3->grid[i][j] == -1);
    }
  }

  assert(p5->size == 5);
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      assert(p5->grid[i][j] == -1);
    }
  }

  puzzle_destroy(p3);
  puzzle_destroy(p5);
}

// also tests puzzle_get_tile
void test_puzzle_set_tile(void) {
  int init[4][4] = {
    { 5, 7, 1, 2 },
    { 13, 9, 3, 4 },
    { 0, 8, 6, 11 },
    { 14, 15, 10, 12 },
  };

   Puzzle *p4 = puzzle_create(4);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      puzzle_set_tile(p4, i, j, init[i][j]);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) {
      int expected = init[i][j];
      int actual = puzzle_get_tile(p4, i, j);
      if (expected != actual) {
        printf("Mismatch at (%d, %d): expected %d, got %d\n", i, j, expected, actual);
        assert(expected == actual);
      }
    }
  puzzle_destroy(p4);
}

void test_handle_K_command(void) {
    // Test unsolved puzzle
    int unsolved_grid[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}
    };
    Puzzle *unsolved_puzzle = puzzle_create(3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            puzzle_set_tile(unsolved_puzzle, i, j, unsolved_grid[i][j]);
        }
    }
    assert(handle_K_command(unsolved_puzzle, 0) == 1);
    puzzle_destroy(unsolved_puzzle);

    // Test solved puzzle
    int solved_grid[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };
    Puzzle *solved_puzzle = puzzle_create(3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            puzzle_set_tile(solved_puzzle, i, j, solved_grid[i][j]);
        }
    }
    assert(handle_K_command(solved_puzzle, 0) == 0);
    puzzle_destroy(solved_puzzle);
}

void test_handle_S_command(void) {
    // Test a valid sliding   
    int grid[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };
    Puzzle *puzzle = puzzle_create(3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            puzzle_set_tile(puzzle, i, j, grid[i][j]);
        }
    }
    assert(handle_S_command(puzzle, 'u') == 1);
    assert(puzzle_get_tile(puzzle, 2, 2) == 0);
    assert(puzzle_get_tile(puzzle, 1, 2) == 6);
    puzzle_destroy(puzzle);
}

int main(void) {
  test_puzzle_create();
  test_puzzle_set_tile();
  test_handle_K_command();
  test_handle_S_command();
  printf("All tests passed!\n");
  return 0;
}
