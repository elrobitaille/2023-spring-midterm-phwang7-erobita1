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
  p->rows = 0;
  p->cols = 0;
  p->bg_image = NULL;

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
    fprintf(stderr, "No puzzle\n");
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

            /* Makes sure that the tile values are correct and valid. */
            if (tile < 0 || tile >= num_tiles) {
                fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
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
  p->rows = p->bg_image->rows / p->size;
  p->cols = p->bg_image->cols / p->size;
  
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
              fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
            }
            return 1;
    }

    /* Grab an error if the direction is invalid or cannot be placed in the certain spot. */
    if (new_row < 0 || new_row >= p->size || new_col < 0 || new_col >= p->size) {
        if (output) {
          fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
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

    /* Say puzzle cannot be moved in a certain direction if error is reached by move_tile function. */
    if (move_tile(p, zero_row, zero_col, dir, 1) != 0) {
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 1;
    }

    /* 
    printf("Puzzle Setup:\n");
    for (int i = 0; i < p->size; i++) {
        for (int j = 0; j < p->size; j++) {
            printf("%d ", puzzle_get_tile(p, i, j));
        }
        printf("\n");
    }
    */
    

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

/* Write puzzle image to image and puzzle configuration to config */
int handle_W_command(FILE *in, Puzzle *p) {
  char image[256];
  char config[256];
  
  /* Get output file names */
  if (fscanf(in, " %s %s", image, config) != 2) {
    fprintf(stderr, "Invalid input\n");
    return 1;
  } 

  if (!p) {
    fprintf(stderr, "No puzzle\n");
    return 1;
  }

  /* If background image hasn't been read, load it with I command */
  if (p->bg_image == NULL) {
    FILE *fp = fopen(image, "rb");
    if (fp == NULL) {
      fprintf(stderr, "Could not open image file '%s'\n", image);
      return 1;
    }

    if (handle_I_command(fp, p) != 0) {
      fprintf(stderr, "No image\n");
      return 1;
    }
    fclose(fp);
  }

  //* Check that dimensions have been read from image */
  if (p->bg_image->cols == 0 || p->bg_image->rows == 0) {
    fprintf(stderr, "Invalid image dimensions\n");
    return 1;
  }

  /* Check bg image dimensions are evenly divisible by puzzle dimensions */
  if (p->bg_image->cols % p->cols != 0 || p->bg_image->rows % p->rows != 0) {
   fprintf(stderr, "Invalid image dimensions\n");
   return 1;
  }

  /* Check puzzle dimensions are greater than zero */
  if (p->cols <= 0 || p->rows <= 0) {
   fprintf(stderr, "Invalid puzzle dimensions\n");
   return 1;
  }

  /* New image instance */
  Image *output_image = (Image *) malloc(sizeof(Image));
  output_image->rows = p->bg_image->rows;
  output_image->cols = p->bg_image->cols;
  output_image->data = (Pixel *) malloc(sizeof(Pixel) * output_image->rows * output_image->cols);

  /* Fill in output image */
  int tile_rows_size = output_image->rows / p->size;
  int tile_cols_size = output_image->cols / p->size;

  for (int i = 0; i < output_image->rows; i++) {
    for (int j = 0; j < output_image->cols; j++) {
      int tile_row = i / tile_rows_size;
      int tile_col = j / tile_cols_size;

    if (tile_row < p->size && tile_col < p->size) {
      int tile_value = p->grid[tile_row][tile_col];

      if (tile_value == 0) {
        output_image->data[i * output_image->cols + j].r = 0;
        output_image->data[i * output_image->cols + j].g = 0;
        output_image->data[i * output_image->cols + j].b = 0;
      }
      /* Copy bg image otherwise */
      else {
        int correct_tile_row = (tile_value - 1) / p->size;
        int correct_tile_col = (tile_value - 1) % p->size;  
        int rowbg = correct_tile_row * tile_rows_size + (i % tile_rows_size);
        int colbg = correct_tile_col * tile_cols_size + (j % tile_cols_size);
        output_image->data[i * output_image->cols + j] = p->bg_image->data[rowbg * p->bg_image->cols + colbg];
      }
    } else {
      output_image->data[i * output_image->cols + j].r = 0;
      output_image->data[i * output_image->cols + j].g = 0;
      output_image->data[i * output_image->cols + j].b = 0;
    }
  }
}

  /* Open output image for writing */
  FILE *imgfile = fopen(image, "wb");
  if (imgfile == NULL) {
    fprintf(stderr, "Could not open output image file '%s'\n", image);
    FreePPM(output_image);
    return 1;
  }
  
  /* Check WritePPM fails to successfully write PPM data */
  if (WritePPM(imgfile, output_image) < 0) {
    fclose(imgfile);
    FreePPM(output_image);
    return 1;
  }
  fclose(imgfile);

  /* Open output config file for writing */
  FILE *output_config = fopen(config, "w");
  if (output_config == NULL) {
    fprintf(stderr, "Could not open output puzzle file '%s'\n", config);
    FreePPM(output_image);
    return 1;
  }

  // Write puzzle data to the config file
  for (int i = 0; i < p->size; i++) {
    for (int j = 0; j < p->size; j++) {
      if (i == p->size - 1 && j == p->size - 1) {
      fprintf(output_config, "%d", p->grid[i][j]);
    } else {
      fprintf(output_config, "%d ", p->grid[i][j]);
      }
    }
  }

  if (fclose(output_config) != 0) {
    fprintf(stderr, "Could not write puzzle data '%s'\n", config);
    return 1;
  }

  /* Clear memory */
  FreePPM(output_image);
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

     /* If theres a new index then recursively solve the puzzle with that direction. */  
    if (new_index != -1) {
        char dir = directions[new_index];
        Puzzle *copy = puzzle_copy(p);

         /* Move the tile again */
         if (move_tile(copy, copy->row_index, copy->col_index, dir, 0) == 0) {
            /* Make recursive to solve puzzle and check for success. */
            int result = solve_puzzle(copy, steps, max_steps, cur_steps + 1, dir);

            /* Solution found if not failure, then add to the steps array and destroy puzzle. */
            if (result != -1) {
                steps[cur_steps] = dir;
                puzzle_destroy(copy);
                return result;
            }
        }
        puzzle_destroy(copy);
    }

    // No results then return -1 as failure. 
    return -1;
}

int handle_V_command(Puzzle *p) {
    // Check for no puzzle, meaning null. 
    if (p == NULL) {
        fprintf(stderr, "No puzzle\n");
        return 1;
    }

    // Initialize array of steps then call solve_puzzle to recursively solve the puzzle.
    char steps[1024] = {0};
  
    // Calls the solve_puzzle recursively with 1024 steps and puts in result. 
    int result = solve_puzzle(p, steps, 1024, 0, '\0');

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

/* Quits the puzzle when "Q" is passed. */
int handle_Q_command(Puzzle *p) {
  /* No puzzle, so destroy it. */
  if (p != NULL) {
    puzzle_destroy(p);
  }
  return 0;
}


