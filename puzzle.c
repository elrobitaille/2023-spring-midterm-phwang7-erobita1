// Edgar Robitaille erobita1
// Patrick Hwang phwang7
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
            printf("C\n");
            /* Check if there was an error for the C command. */
            if (handle_C_command(game_input, &p) != 0) {
              return 1; 
            }
            break;
        case 'T':
            /* Grab the T command and check the tiles. */
            printf("T\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
            return 1;
            }
            if (handle_T_command(game_input, p) != 0) {
              return 1;
            }
            break; 
        case 'I': 
            printf("I\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            } 
            if (handle_I_command(game_input, p) != 0) {
              return 1;
            }
            break;
        case 'P':
            printf("P\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
            return 1;
            }
            handle_P_command(p);
            break;
        case 'W':
            printf("W\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            } 
            if (handle_W_command(game_input, p) != 0) {
              return 1;
            }
            break;
        case 'S':
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            }
            // Grab the direction and make sure it is size 1, only one letter for dir. 
            char direction;
            if (fscanf(game_input, " %c", &direction) != 1) {
              fprintf(stderr, "Invalid input\n");
              return 1;
            }
            if (handle_S_command(p, direction) != 0) {
              return 1;
            }
            break;
        case 'K':
            printf("K\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            }
            handle_K_command(p);
            break;
        case 'V':
            printf("V\n");
           /* 
           if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            }
            handle_V_command(p);
           */ 
            break;
        case 'Q':
            /* Exit in case of a Q "Quit" command. */
            printf("Q\n");
            handle_Q_command(p);
            break;
        default: 
            /* If no accurate command letter is given, catch an invalid command error. 
            Or, if input command is not a valid uppercase character, skip to the next command. */
            fprintf(stderr, "Invalid command '%c'\n", input_command);
            return 1; 
      }
    }

    fclose(game_input);
  }

 
}
