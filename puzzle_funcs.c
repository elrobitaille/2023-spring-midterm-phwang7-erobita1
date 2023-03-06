#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "puzzle.h"


Puzzle *puzzle_create(int size) {
  /* Allocate the memory for Puzzle struct and make sure it is valid */
  Puzzle *p = malloc(sizeof(Puzzle)); 
  assert(p != NULL); 

  p->size = size;
  p->grid = malloc(sizeof(int*) * size); // Allocate memory for 2D array of size x size (n x n) square
  if (p->grid == NULL) {
    free(p);
    return NULL;
  }

  /* Allocate memory for each row and initialize, make sure it is valid. */
  for (int i = 0; i < size; i++) {
    p->grid[i] = malloc(sizeof(int) * size);
    assert(p->grid[i] != NULL); 

    /* Initialize each cell to -1.  */
    for (int j = 0; j < size; j++) {
      p->grid[i][j] = -1;
    } 
  }
 
  return p;
}

void puzzle_destroy(Puzzle *p) {
  /* Get rid of the puzzle if it is invalid and free the memory. */
    if (p != NULL) {
      for (int i = 0; i < p->size; i++) {
        free(p->grid[i]);
      }
        free(p->grid);
        free(p);
    }
}

/* Set the tile in the puzzle into grid. */
void puzzle_set_tile(Puzzle *p, int col, int row, int value) {
  if (p == NULL) {
    fprintf(stderr, "Invalid puzzle\n");
    return;
  }
  p->grid[row][col] = value;
}

/* Grab the value at the specified row or column in grid 2D array. */
int puzzle_get_tile(const Puzzle *p, int col, int row) {
  return p->grid[row][col];
}

int handle_C_command(FILE *in, Puzzle **p) {
  int size = 0; //size of the puzzle size x size (square)
  int size_scan = fscanf(in, " %d\n", &size);

  /* Catch puzzle size error or missing/invalid command for C. */
  if (size < 2 || size > 20) {
    fprintf(stderr, "Invalid puzzle size\n");
    return 1;
  }

  if (size_scan != 1) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  /* Allocate memory space for the new puzzle. */ 
  Puzzle *new_puzzle = puzzle_create(size);
  if (new_puzzle == NULL) {
    fprintf(stderr, "Failed to allocate memory for puzzle\n");
    return 1;
  }

  puzzle_destroy(*p); // Destroys any existing puzzle to allocate memory for new puzzle
  *p = new_puzzle;

  return 0;
}

int handle_T_command(FILE *in, Puzzle *p) {
    /* Check if there was an error in creating the puzzle for T command. */
    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }
    /* Calculate the number of tiles based on the size of the puzzle */
    int num_tiles = p->size * p->size; // Number of tiles is equal to size squared (square shape puzzle)
    int tile_scan = 0; // This is the number of tiles that have been scanned
    int tile; // This is the current tile being scanned
    int tile_input = 1; // Result of the fscanf input 

    /* Read in tiles and populate puzzle */
    while (tile_scan < num_tiles && tile_input == 1) {
        tile_input = fscanf(in, "  %d", &tile);
        printf("%d\n", tile);

        /* Makes sure that the tile values are correct and valid. */
        if (tile < 0 || tile >= num_tiles) {
            fprintf(stderr, "Invalid tile value\n");
            return 1;
        }

        if (tile_input != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
        }

        /* Place the tile in the puzzle. */
        int row = (tile - 1) / p->size;
        int col = (tile - 1) % p->size;
        /* Avoid memory leakage and going out of bounds on the array. */
        if (row >= 0 && row < p->size && col >= 0 && col < p->size) {
            puzzle_set_tile(p, row, col, tile_scan);
        }
        tile_scan++;
    }

  /* Checks to make sure that all the tiles were iterated through. */
  if (tile_scan != num_tiles) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  return 0;
}

int handle_Q_command(Puzzle *p) {
  if (p != NULL) {
    puzzle_destroy(p);
  }
  return 0;
}
