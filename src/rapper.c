/**************************************************************************
 *   rapper.c  --  This file is part of rapper.                           *
 *                                                                        *
 *   Copyright (C) 2025 Chen Zeng                                         *
 *                                                                        *
 *   Rapper is free software: you can redistribute it and/or modify it    *
 *   under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or    *
 *   (at your option) any later version.                                  *
 *                                                                        *
 *   Rapper is distributed in the hope that it will be useful, but        *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU     *
 *   General Public License for more details.                             *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "usage.h"

#define DEFAULT_MAX_LINE_LEN 72

int main(int argc, char **argv) {
  if (argc == 1) {
    usage(1);
    return EXIT_FAILURE;
  }

  int vflag = 0;
  char *nvalue = NULL;
  char *ovalue = NULL;

  int c;
  // int opterr = 0;

  while ((c = getopt(argc, argv, "vn:o:")) != -1) {
    switch (c) {
    case 'v':
      vflag = 1;
      break;
    case 'n':
      nvalue = optarg;
      break;
    case 'o':
      ovalue = optarg;
      break;
    case '?':
      if (optopt == 'n' || optopt == 'o')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }
  }

  long MAXLINELEN = DEFAULT_MAX_LINE_LEN;
  if (nvalue != NULL) {
    MAXLINELEN = strtol(nvalue, NULL, 10);
  }

  if (optind >= argc) {
    puts("ERROR: no input file.");
    usage(0);
    return EXIT_FAILURE;
  }

  char *filepath = argv[optind];

  FILE *f_input = fopen(filepath, "r");
  char *readbuffer = malloc(2048 * sizeof(char));

  FILE *f_output = stdout;
  if (ovalue != NULL) {
    f_output = fopen(ovalue, "w");
  }

  if (vflag == 1) {
    puts("Verbose mode is on.");
    if (nvalue == NULL) {
      printf("All lines are wrapped at the default %d columns.\n",
             DEFAULT_MAX_LINE_LEN);
    } else {
      printf("All lines are wrapped at %li columns.\n", MAXLINELEN);
    }
    printf("Input file: %s\n", filepath);
    if (ovalue == NULL) {
      puts("Output will be written to STDIN.");
    } else {
      printf("Output will be saved to %s.", ovalue);
    }
  }

  int originallinec = 0, newlinec = 0;

  for (; fgets(readbuffer, 2048, f_input); originallinec++) {
    // printf("===== Line %d =====\n", i);

    int linelen = 0;
    int linewordc = 0;

    for (char *seg = strtok(readbuffer, " "); seg != NULL;
         seg = strtok(NULL, " ")) {
      // N of characters in a word.
      int wordlen = (int)strlen(seg);

      // The first word is printed regardless of the length.
      if (linewordc == 0) {
        fprintf(f_output, "%s", seg);
        linelen += wordlen;
        linewordc++;
        newlinec++;
        continue;
      }

      // If current line length + next word length <= MAXLINELEN, print the word
      // preceded by a space. No space if this is the first word of the line.
      // Line word count increases by 1, and line length increases by word
      // length, plus 1 if space is added.
      // Else, a new line is added and both counters are reset.
      if (linelen + wordlen + 1 <= MAXLINELEN) {
        fprintf(f_output, " %s", seg);
        linelen += wordlen + 1;
        linewordc++;
      } else {
        // Current line length is printed.
        // printf("\n");
        // for (int j = 0; j < linelen - 1; j++)
        //   printf(" ");
        // printf("^\n");
        // for (int j = 0; j < linelen - 1; j++)
        //   printf(" ");
        // printf("%d\n", linelen);
        fprintf(f_output, "\n%s", seg);
        linelen = wordlen;
        newlinec++;
        linewordc = 1;
      }
    }

    // printf("\n");
  }
  printf("\n");

  if (vflag == 1)
    printf("\nOriginal N lines: %d\nNew N lines:%d\n", originallinec, newlinec);

  fclose(f_input);
  fclose(f_output);
}
