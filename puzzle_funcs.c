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
int puzzle_get_tile(const Puzzle *p, int row, int col) {
  return p->grid[row][col];
}

/* Function that finds where the zero is in the 2D array. */
int puzzle_zero_tile(Puzzle *p, int *row, int *col) {
    // Checks for a null row, col, or puzzle pointer. 
    if (p == NULL || row == NULL || col == NULL) {
      fprintf(stderr, "No puzzle");
      return 1;
    }

    // Searches for the zero in the 2D array and returns result. 
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            if (p->grid[i][j] == 0) {
                *row = i;
                *col = j;
                return 0;
            }
        }
    }
    return 1;
}

/* Opposite direction finder for the solve_puzzle function (helper function).  */
char opposite_direction(char dir) {
    switch (dir) {
        case 'u':
            return 'd';
        case 'd':
            return 'u';
        case 'l':
            return 'r';
        case 'r':
            return 'l';
        default:
            return '\0'; // Invalid direction, return null terminator
    }
}

/* Creates a new puzzle with the same values as the given puzzle. */
Puzzle *puzzle_copy(Puzzle *p) {
    /* Create new puzzle with same size as original. */
    Puzzle *copy = puzzle_create(p->size);
    /* Copy values of tiles from the original puzzle to new one. */ 
    for (int row = 0; row < p->size; row++) {
        for (int col = 0; col < p->size; col++) {
            int value = puzzle_get_tile(p, row, col);
            puzzle_set_tile(copy, row, col, value);
        }
    }
    // Set the row and column indices.
    copy->row_index = p->row_index;
    copy->col_index = p->col_index;

    return copy;
}

// Manhattan distance helper function for solve_puzzle implementation. 
int manhattan_distance(Puzzle *p) {
    int distance = 0;
    for (int i = 0; i < p->size; i++) {
      for (int j = 0; j < p->size; j++) {
          int val = p->grid[i][j];
          if (val != 0) {
              int target_row = (val - 1) / p->size;
              int target_col = (val - 1) % p->size;
              distance += abs(target_row - i) + abs(target_col - j);
          }
        }
    }
    return distance;
}


int handle_C_command(FILE *in, Puzzle **p) {
  int size = 0; //size of the puzzle size x size (square)
  int size_scan = fscanf(in," %d\n", &size);

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

  FreePPM(p->bg_image);
  p->bg_image = new_image;

  return 0;
}

/* Move a tile in the specified direction, will be called by handle_S_command function. */
int move_tile(Puzzle *p, int row, int col, char dir, int output) {
    int new_row = row, new_col = col;

    /* Switch case for each direction: up, down, left, right, for the game to move the tile. */
    switch (dir) {
        case 'u':
            new_row = row + 1;
            break;
        case 'd':
            new_row = row - 1;
            break;
        case 'l':
            new_col = col + 1;
            break;
        case 'r':
            new_col = col - 1;
            break;
        default:
            // Invalid case that does not use any correct letter movement. 
            if (output) {
              fprintf(stderr, "Puzzle cannot be moved in specified direction1\n");
            }
            return 1;
    }

    /* Grab an error if the direction is invalid or cannot be placed in the certain spot. */
    if (new_row < 0 || new_row >= p->size || new_col < 0 || new_col >= p->size) {
        if (output) {
          fprintf(stderr, "Puzzle cannot be moved in specified direction2\n");
        }
        return 1;
    }

    int next_value = puzzle_get_tile(p, new_row, new_col);
    puzzle_set_tile(p, row, col, next_value);
    puzzle_set_tile(p, new_row, new_col, 0);
    

    p->row_index = new_row;
    p->col_index = new_col;


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
    if (puzzle_zero_tile(p, &zero_row, &zero_col) != 0) {
        fprintf(stderr, "Invalid Input\n");
        return 1;
    }

    printf("Direction: %c\n", dir);
    printf("Zero tile at: (%d, %d)\n", zero_row, zero_col);


    /* Say puzzle cannot be moved in a certain direction if error is reached by move_tile function. */
    if (move_tile(p, zero_row, zero_col, dir, 1) != 0) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction5\n");
        return 1;
    }

    printf("After move:\n");
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", puzzle_get_tile(p, i, j));
        }
        printf("\n");
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
  char image[256]; // instructions say to use 255 as max + null terminator
  char config[256];
  if (fscanf(in, " %s %s", image, config) != 2) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  } 

  if (!p) {
    fprintf(stderr, "No puzzle\n");
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

  /* Create new Image instance */
  Image *img = malloc(sizeof(Image));
  if (img == NULL) {
    fprintf(stderr, "No image\n");
    return 1;
  }

  img->data = malloc(p->rows * p->cols * sizeof(Pixel));
  img->rows = p->rows;
  img->cols = p->cols;

  //still need to add RGB stuff, figuring that out currently

  return 0;
  }

int handle_K_command(Puzzle *p, int output) {
  /*Takes in puzzle and output variable, which if output = 1 then "Solved" or "Not solved" is printed,
  otherwise it does not print anything. Output is 1 in puzzle.c, while it is 0 for solve_puzzle function
  because we don't want anything to be printed in that scenario. */
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
          if (expected <= n && p->grid[i][j] != expected) {
                if (output == 1) {
                    printf("Not solved\n");
                }
                return 1;
            }
            if (i == p->size - 1 && j == p->size - 1 && puzzle_get_tile(p, i, j) != 0) {
                if (output == 1) {
                    printf("Not solved\n");
                }
                return 1;
            }
            expected++;
            if (expected > n) {
                break;
            }
        }
        if (expected > n) {
            break;
        }
    }
    /* Successfully solved the puzzle, return 0 and solved.  */
    if (output == 1) {
      printf("Solved\n");
    }
    return 0;
}

int solve_puzzle(Puzzle *p, char steps[], int max_steps, int cur_steps, char prev_move) {
    // If the puzzle is solved, exit and print num steps. 
    if (handle_K_command(p, 0) == 0) {
      steps[cur_steps] = '\0'; //End the array and terminate with the null terminator. 
      return cur_steps;
    }

    // If exceeded number of steps then there's no solution. 
    if (cur_steps >= max_steps) {
      fprintf(stderr, "No solution found\n");
      return -1;
    }

    // All of the characters included, try each move in this order, then run recursively. 
    char directions[] = {'u', 'd', 'l', 'r'};
    int optimal_distance = INT_MAX; 
    int new_index = -1;

    // Loop through each direction, 4 characters so i < 4. 
    for (int i = 0; i < 4; i++) {
        char dir = directions[i];

        /* This optimizes the brute force + manhattan heuristic approach as it avoids redundancy. */
        if (dir == opposite_direction(prev_move)) {
            continue; //Skip this direction because the opposite is already tried, so it is pointless to do same.
        }

        /* Create a copy as said by the pseudocode, use puzzle_copy function to create copy. */
        Puzzle *copy = puzzle_copy(p);
        /* Move the tile in current direction. */
        if (move_tile(copy, copy->row_index, copy->col_index, dir, 0) == 0) {
           // Use the manhattan distance function to find the manhattan distance after moving.
           int distance = manhattan_distance(copy);
           // This will update the "new" index and determine the best distance (ie if distance is smaller its better).
           if (distance < optimal_distance) {
              optimal_distance = distance;
              new_index = i;
           }
        }
        // Get rid of the puzzle copy. 
        puzzle_destroy(copy);
    }
    return -1;
}

int handle_V_command(Puzzle *p) {
    // Check for no puzzle, meaning null. 
    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }

    // Initialize array of steps then call solve_puzzle to recursively solve the puzzle.
    char steps[256] = {0};
  
    // Calls the solve_puzzle recursively with 256 steps and puts in result. 
    int result = solve_puzzle(p, steps, 256, 0, '\0');

    /* If result is invalid then there is no solution. */
    if (result < 0) {
        fprintf(stderr, "No solution found\n");
        return 1;
    }

    /* Iterate through the successes and print out the corresponding directions.*/
    for (int i = 0; i < result; i++) {
        printf("S %c\n", steps[i]);
    }

    return 0;
}

int handle_Q_command(Puzzle *p) {
  if (p != NULL) {
    puzzle_destroy(p);
  }
  return 0;
}


