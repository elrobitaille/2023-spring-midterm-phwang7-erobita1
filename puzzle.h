// Edgar Robitaille erobita1
// Patrick Hwang phwang7
#ifndef PUZZLE_H
#define PUZZLE_H

#include "ppm_io.h" // for Image data type

typedef struct {
  int size; // Size of the 2D image array, number rows or columns 
  int **grid; // 2D array of integers which are the puzzle tiles
  int row_index; // The row index of the tile
  int col_index; // The column index of the tile 
  int rows;
  int cols;
  Image *bg_image; // Background image 
} Puzzle;

/* Main puzzle functions to create and destroy the puzzle. */
Puzzle *puzzle_create(int size);
void puzzle_destroy(Puzzle *p);
void puzzle_set_tile(Puzzle *p, int col, int row, int value);
int puzzle_get_tile(const Puzzle *p, int col, int row);
int puzzle_zero_tile(Puzzle *p, int *row, int *col);
char opposite_direction(char dir);
Puzzle *puzzle_copy(Puzzle *p);
int manhattan_distance(Puzzle *p);

/* Commands to handle each first letter command case. */
int handle_C_command(FILE *in, Puzzle **p);
int handle_T_command(FILE *in, Puzzle *p);
int handle_I_command(FILE *in, Puzzle *p);
int move_tile(Puzzle *p, int row, int col, char dir, int output);
int handle_P_command(Puzzle *p);
int handle_W_command(FILE *in, Puzzle *p);
int handle_S_command(Puzzle *p, char dir);
int handle_K_command(Puzzle *p, int output);
int solve_puzzle(Puzzle *p, char steps[], int max_steps, int cur_steps, char prev_move);
int handle_V_command(Puzzle *p);
int handle_Q_command(Puzzle *p);

#endif // PUZZLE_H
