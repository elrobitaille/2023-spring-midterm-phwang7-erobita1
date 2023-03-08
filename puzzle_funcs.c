#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "ppm_io.h"
#include "puzzle.h"


Puzzle *puzzle_create(int size) {
  /* Allocate the memory for Puzzle struct and make sure it is valid */
  Puzzle *p = calloc(1, sizeof(Puzzle)); 
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

/* Get rid of the puzzle if it is invalid and free the memory. */
void puzzle_destroy(Puzzle *p) {
    if (p != NULL) {
      for (int i = 0; i < p->size; i++) {
        free(p->grid[i]);
      }
        free(p->grid);
        free(p);
    }
}

/* Set the tile in the puzzle into grid. */
void puzzle_set_tile(Puzzle *p, int row, int col, int value) {
    if (p == NULL) {
        fprintf(stderr, "Invalid puzzle\n");
        return;
    }
    if (row >= p->size || col >= p->size) {
        fprintf(stderr, "Invalid tile value\n");
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

    /* Read tiles and populate the puzzle with the corresponding values. */
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            tile_input = fscanf(in, " %d", &tile);
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
            puzzle_set_tile(p, i, j, tile);
            tile_scan++;
        }
    }

    /* Checks to make sure that all the tiles were iterated through. */
    if (tile_scan != num_tiles) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    return 0;
}

/* Loads background image from PPM image file */
int handle_I_command(FILE *in, Puzzle *p) {
  char image_name[256];
  /* Throws error if invalid input */
  if (fscanf(in, "%s", image_name) != 1) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  FILE *fp = fopen(image_name, "rb");
  /* Throws error if image file cannot be opened */
  if (fp == NULL) {
    fprintf(stderr, "Could not open image file '%s'\n", image_name);
    return 1;
  }

  /* Opens PPM file and reads it */
  Image *new_image = ReadPPM(fp);
  fclose(fp);

  /* Throws error if image file cannot be opened */
  if (new_image == NULL) {
    fprintf(stderr, "Could not open image file '%s'\n", image_name);
    return 1;
  }

  /* Destroy any existing background images. */
  if (p->bg_image != NULL) {
    FreePPM(p->bg_image);
  }

  p->bg_image = new_image;

  return 0;
}

/* Move a tile in the specified direction, will be called by handle_S_command function. */
int move_tile(Puzzle *p, int row, int col, char dir) {
    int size = p->size;
    int temp = puzzle_get_tile(p, col, row);
    int new_row = row, new_col = col;

    /* Switch case for each direction: up, down, left, right, for the game to move the tile. */
    switch (dir) {
        case 'u':
            new_row = row - 1;
            break;
        case 'd':
            new_row = row + 1;
            break;
        case 'l':
            new_col = col - 1;
            break;
        case 'r':
            new_col = col + 1;
            break;
        default:
            printf("Puzzle cannot be moved in specified direction\n");
            return 1;
    }

    /* Grab an error if the direction is invalid or cannot be placed in the certain spot. */
    if (new_row < 0 || new_row >= size || new_col < 0 || new_col >= size) {
        printf("Puzzle cannot be moved in specified direction\n");
        return 1;
    }

    int value = puzzle_get_tile(p, new_col, new_row);
    puzzle_set_tile(p, new_col, new_row, value);
    puzzle_set_tile(p, col, row, temp);

    return 0;
}

int handle_S_command(Puzzle *p, FILE* in) {
  
}

void handle_P_command(Puzzle *p) {
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", p->grid[i][j]);
        }
    }
    printf("\n");
}

int handle_Q_command(Puzzle *p) {
  if (p != NULL) {
    puzzle_destroy(p);
  }
  return 0;
}


