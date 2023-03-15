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
    //Create a character for the command, make a null puzzle, then boolean to show if puzzle is done. 
    char command;
    Puzzle *p = NULL;
    int puzzle_created = 0;
    
    /* C will always be first as shown in the input files, so start with C and make sure it is valid. */
    while (scanf(" %c", &command) == 1) {
      if (!puzzle_created && command != 'C') {
        fprintf(stderr, "Invalid input\n");
        return 1;
      }

      /* Now, we can have a switch case for each character type and handle each accordingly to stdin. */
      switch (command) {
        case 'C':
          if (puzzle_created) {
            fprintf(stderr, "Invalid input\n");
            return 1;
          }

          // Make sure the scan size is valid, just 1. 
          int puzzle_size;
          if (scanf("%d", &puzzle_size) != 1) {
            fprintf(stderr, "Invalid input\n");
            return 1;
          }

          // Check if puzzle has a wrong size. 
          if (puzzle_size <= 0 || puzzle_size > 255) {
            fprintf(stderr, "Invalid puzzle size\n");
            return 1;
          }

          // Create a puzzle and make sure it is valid. 
          p = puzzle_create(puzzle_size);
          if (p == NULL) {
            fprintf(stderr, "No puzzle\n");
            return 1;
          }

          puzzle_created = 1; 
          break;

        case 'T':
          /* Grab the T command and check the tiles. */
          if (handle_T_command(stdin, p) != 0) {
            return 1;
          }
          break;

        case 'I':
          /* Initializes the background and sets the image as specified. */
          if (handle_I_command(stdin, p) != 0) {
            return 1;
          }
          break;

        case 'P':
           /* Print the current orientation of the puzzle in a single line. */
          handle_P_command(p);
          break;

        case 'W':
          /* Load the image and add RGB values, and slice up the image. */
          if (handle_W_command(stdin, p) != 0) {
            return 1;
          }
          break;

        case 'S':
          /* Slides the 0 tile in the direction specified in the file. */
          char direction;
          if (scanf(" %c", &direction) != 1) {
            fprintf(stderr, "Invalid input\n");
             return 1;
          }
          if (handle_S_command(p, direction) != 0) {
            return 1;
          }
          break;

        case 'K':
          /* Print whether or not the current configuration of the puzzle is solved. */
          handle_K_command(p, 1);
          break;

        case 'V':
          /* Recursively solve the puzzle on its own using brute force approach. */
          handle_V_command(p);
          break;

        case 'Q':
        /* Exit in case of a Q "Quit" command. */
          handle_Q_command(p);
          exit(0);

        default:
          /* If no accurate command letter is given, catch an invalid command error. 
            Or, if input command is not a valid uppercase character, skip to the next command. */
          fprintf(stderr, "Invalid command '%c'\n", command);
          return 1;
      }
    }
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
            /* Initializes the background and sets the image as specified. */
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
            /* Print the current orientation of the puzzle in a single line. */
            printf("P\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
            return 1;
            }
            handle_P_command(p);
            break;
        case 'W':
            /* Load the image and add RGB values, and slice up the image. */
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
            /* Slides the 0 tile in the direction specified in the file. */
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
            /* Print whether or not the current configuration of the puzzle is solved. */
            printf("K\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            }
            handle_K_command(p, 1);
            break;
        case 'V':
            /* Recursively solve the puzzle on its own using brute force approach. */
            printf("V\n");
            if (p == NULL) {
              fprintf(stderr, "No puzzle\n");
              return 1;
            }
            handle_V_command(p);
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
