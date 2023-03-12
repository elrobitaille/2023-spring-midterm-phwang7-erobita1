// Edgar Robitaille erobita1
// Patrick Hwang phwang7
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

/* Function that finds where the zero is in the 2D array. */
int puzzle_zero_tile(Puzzle *p, int tile, int *row, int *col) {
    // Checks for a null row, col, or puzzle pointer. 
    if (p == NULL || row == NULL || col == NULL) {
      fprintf(stderr, "No puzzle");
      return 1;
    }

    // Searches for the zero in the 2D array and returns result. 
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            if (p->grid[i][j] == tile) {
                *row = i;
                *col = j;
                return 0;
            }
        }
    }
    return 1;
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
  char image_name[255];
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

  p->bg_image = new_image;

  return 0;
}

/* Move a tile in the specified direction, will be called by handle_S_command function. */
int move_tile(Puzzle *p, int row, int col, char dir) {
    int new_row = row, new_col = col;

    /* Switch case for each direction: up, down, left, right, for the game to move the tile. */
    switch (dir) {
        case 'u':
            new_col = col + 1;
            break;
        case 'd':
            new_col = col - 1;
            break;
        case 'l':
            new_row = row + 1;
            break;
        case 'r':
            new_row = row - 1;
            break;
        default:
            // Invalid case that does not use any correct letter movement. 
            fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
            return 1;
    }

    /* Grab an error if the direction is invalid or cannot be placed in the certain spot. */
    if (new_row < 0 || new_row >= p->size || new_col < 0 || new_col >= p->size) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 1;
    }

    // Set the puzzle tiles using the getter and setter functions. 
    int next_value = puzzle_get_tile(p, new_row, new_col);

    /* Set the tiles accordingly using the set_tile function. */
    puzzle_set_tile(p, col, row, next_value);
    puzzle_set_tile(p, new_col, new_row, 0);

    return 0;
}

int handle_S_command(Puzzle *p, char dir) {
    /* Make sure that the direction is a valid letter. */
    if (dir != 'u' && dir != 'd' && dir != 'l' && dir != 'r') {
      fprintf(stderr, "Invalid input\n");
      return 1;  
    }

    /* Initialize the zero location in the 2D array, then use the puzzle_zero_tile function to find the index. */
    int zero_row = -1, zero_col = -1;
    if (puzzle_zero_tile(p, 0, &zero_col, &zero_row) != 0) {
        fprintf(stderr, "Invalid Input\n");
        return 1;
    }

    printf("Updated puzzle:\n");
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", puzzle_get_tile(p, j, i));
        }
        printf("\n");
    }

    /* Say puzzle cannot be moved in a certain direction if error is reached by move_tile function. */
    if (move_tile(p, zero_row, zero_col, dir) != 0) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 1;
    }
  
    return 0;

}

void handle_P_command(Puzzle *p) {
  /* Iterate through the 2D array and print the values in a single line. */
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", p->grid[i][j]);
        }
    }
    printf("\n");
}

/* Write puzzle image to ppm_out and puzzle configuration to txt_out */
int handle_W_command(FILE *in, Puzzle *p) {
  char image[255]; // instructions say to use 255 as max 
  char config[255];
  if (fscanf(in, " %s %s", image, config) != 2) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  } 

  printf("image = %s, config = %s\n", image, config);

  /* If background image hasn't been read, load it with I command */
  if (p->bg_image == NULL) {
    FILE *fp = fopen(image, "rb");
    if (fp == NULL) {
      fprintf(stderr, "Could not open image file '%s'\n", image);
      return 1;
    }

  if (handle_I_command(fp, p) != 0) {
      fprintf(stderr, "No image\n");
      fclose(fp);
      return 1;
  }
  fclose(fp);
  }

  /* Check that dimensions have been read from image */
  if (p->bg_image->cols == 0 || p->bg_image->rows == 0) {
    fprintf(stderr, "Invalid image dimensions\n");
    return 1;
  }

  /* Read dimensions from PPM file */
  int width = p->bg_image->cols;
  int height = p->bg_image->rows;

  if (width == 0 || height == 0) {
    fprintf(stderr, "Invalid image dimensions\n");
    return 1;
  }

  p->cols = width / p->size;
  p->rows = height / p->size;

  if (width % p->size != 0 || height % p->size != 0) {
    fprintf(stderr, "Invalid image dimensions\n");
    return 1;
  }

  

   return 0;
  }

int handle_K_command(Puzzle *p) {
  // Make sure that the puzzle is defined and is not null, prints No puzzle if there is error, returns 1.
  if (!p) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }
    // Puzzles always starts at 1, then ends at zero. So the first expected value is 1. 
    int expected = 1;
    int n = p->size * p->size - 1;  // Doesn't count the last element which should be 0, so subtract 1.

    // Iterates through the 2D array and makes sure that it is in numerical order (minus the 0 at the end).
    for (int i = 0; i < p->size; i++) {
      for (int j = 0; j < p->size; j++) {
          /* Basically checks that this current position is not equal to the
          expected value, then checks that the current position is not in the last row or last column,
          and it is not the 0 tile (the ending). If true, then puzzle isn't solved. */
          if (p->grid[i][j] != expected || (i != p->size-1 && j != p->size-1 && p->grid[i][j] == 0)) {
              printf("Not solved\n");
              return 1;
            }
            expected++; //increment the expected value 
            if (expected > n) { // checks that expected value hasnt exceeded max value allowed.
              break;
            }
        }
        if (expected > n) { // checking same condition outside first for loop
          break;
        }
    }
    /* Successfully solved the puzzle, return 0 and solved.  */
    printf("Solved\n");
    return 0;
}

int handle_Q_command(Puzzle *p) {
  if (p != NULL) {
    puzzle_destroy(p);
  }
  return 0;
}


