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

int puzzle_zero_tile(Puzzle *p, int tile, int *row, int *col) {
    if (p == NULL || row == NULL || col == NULL) {
      fprintf(stderr, "No puzzle");
      return 1;
    }

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

  /* Free memory allocated by ReadPPM */
  FreePPM(p->bg_image);

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
            fprintf(stderr, "Puzzle cannot be moved in specified direction1\n");
            return 1;
    }

    printf("new_row: %d, new_col: %d\n", new_row, new_col);
    printf("Puzzle size: %d\n", p->size);

    /* Grab an error if the direction is invalid or cannot be placed in the certain spot. */
    if (new_row < 0 || new_row >= p->size || new_col < 0 || new_col >= p->size) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction3\n");
        return 1;
    }

    /* 
    // Set the puzzle tiles using the getter and setter functions. 
    int next_value = puzzle_get_tile(p, new_row, new_col);

    // Next tile is not empty so there is an error, or the zero can't be moved there. 
    if (next_value != -1) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction4\n");
        return 1; 
    }
    */
    
    int next_value = puzzle_get_tile(p, new_row, new_col);
    puzzle_set_tile(p, col, row, next_value);
    puzzle_set_tile(p, new_col, new_row, 0);

    return 0;
}

int handle_S_command(Puzzle *p, char dir) {
    if (dir != 'u' && dir != 'd' && dir != 'l' && dir != 'r') {
      fprintf(stderr, "Invalid input1\n");
      return 1;  
    }

    int zero_row = -1, zero_col = -1;
    if (puzzle_zero_tile(p, 0, &zero_col, &zero_row) != 0) {
        fprintf(stderr, "Invalid Input2\n");
        return 1;
    }
    printf("Zero tile: (%d, %d)\n", zero_row, zero_col);

    printf("Updated puzzle:\n");
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", puzzle_get_tile(p, j, i));
        }
        printf("\n");
    }

    if (move_tile(p, zero_row, zero_col, dir) != 0) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction5\n");
        return 1;
    }

    printf("%c\n", dir);
  
    return 0;

}

void handle_P_command(Puzzle *p) {
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


