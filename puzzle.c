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

    /* Gather the first line command and perform the corresponding function. */
    char input_command; 
    while (fscanf(game_input, " %c", &input_command) == 1) {
      switch (input_command) {
        case 'C':
            printf("C\n");
            int size;  //size of the puzzle size x size (square)
            int size_scan = fscanf(game_input, "%d", &size);
            printf("size scan: %d", size);
            //handle_C_command(game_input, &puzzle);
            break;
        case 'T':
            break; 
        case 'I': 
            break;
        case 'P':
            break;
        case 'W':
            break;
        case 'S':
            break;
        case 'K':
            break;
        case 'V':
            break;
        case 'Q':
            break;
      }
    }

    fclose(game_input);
  }

 


  // TODO: implement
}
