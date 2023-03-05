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
    Puzzle *p = NULL; // declare the puzzle pointer

    while (fscanf(game_input, " %c", &input_command) == 1) {
      switch (input_command) {
        case 'C':
            /* Check if there was an error for the C command. */
            if (handle_C_command(game_input, &p) != 0) {
              return 1; 
            }
          
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
        default: 
            /* If no accurate command letter is given, catch an invalid command error. 
            Or, if input command is not a valid uppercase character, skip to the next command. */
            if (input_command < 'A' || input_command > 'Z') {
              break;
            }
            fprintf(stderr, "Invalid command '%c'\n", input_command);
            return 1; 
      }
    }

    fclose(game_input);
  }

 


  // TODO: implement
}
