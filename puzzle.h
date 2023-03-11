#ifndef PUZZLE_H
#define PUZZLE_H

#include "ppm_io.h" // for Image data type

typedef struct {
  // ...fields go here...
  int size; // Size of the 2D image array, number rows or columns 
  int **grid; // 2D array of integers which are the puzzle tiles
  int row_index; // The row index of the tile
  int col_index; // The column index of the tile 
  int rows;
  int cols;
  int *board;
  Image *bg_image; // Background image 
} Puzzle;

// TODO: function declarations
Puzzle *puzzle_create(int size);
void puzzle_destroy(Puzzle *p);
void puzzle_set_tile(Puzzle *p, int col, int row, int value);
int puzzle_get_tile(const Puzzle *p, int col, int row);
/* Commands to handle each first letter command case. */
int handle_C_command(FILE *in, Puzzle **p);
int handle_T_command(FILE *in, Puzzle *p);
int handle_I_command(FILE *in, Puzzle *p);
int handle_W_command(FILE *in, Puzzle *p);
int handle_V_command(FILE *in, Puzzle *p);
int handle_K_command(Puzzle *p);
int handle_Q_command(Puzzle *p);
void handle_P_command(Puzzle *p);
int move_tile(Puzzle *p, int row, int col, char dir);
int handle_S_command(Puzzle *p, char dir);


#endif // PUZZLE_H
