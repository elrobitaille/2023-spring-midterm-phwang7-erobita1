#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "puzzle.h"

int main(int argc, char **argv) {
  if (argc < 1 || argc > 2) {
    fprintf(stderr, "Usage: ./puzzle [<command file>]\n");
    return 1;
  }
  
  /* This is for the case where the user inputs only 1 argument (./puzzle) and does
  not provide a file for the code to run through, so the user can manually play the 
  game. */
  if (argc == 1) {
    printf("%d", argc);
  }

  /* This is the case for directly reading from the file, provided in the format ./puzzle input.txt. */
  if (argc == 2) {
    FILE* game_input = fopen(argv[1], "r"); 
    /* Reads the input file and makes sure that it has valid input. */
    if (game_input == NULL) {
      fprintf(stderr, "Error involving input file, %s", argv[1]);
      return 1; //indicate error caught
    }
    
  }


  // TODO: implement
}
