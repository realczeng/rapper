/**************************************************************************
 *   wrap.c  --  This file is part of rapper.                             *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* f_input = NULL;
static FILE* f_output = NULL;

char* readbuffer = NULL;

static void open_files(char* input, char* output);

int wrap_simple(char* filepath, char* ovalue, long MAXLINELEN, int vflag) {
  open_files(filepath, ovalue);

  int originallinec = 0, newlinec = 0;

  for (; fgets(readbuffer, 2048, f_input); originallinec++) {
    // printf("===== Line %d =====\n", i);

    int linelen = 0;
    int linewordc = 0;

    for (char* seg = strtok(readbuffer, " "); seg != NULL;
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

      // If current line length + next word length <= MAXLINELEN,
      // print the word preceded by a space. No space if this is the
      // first word of the line. Line word count increases by 1, and
      // line length increases by word length, plus 1 if space is
      // added. Else, a new line is added and both counters are
      // reset.
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

  if (vflag == 1) {
    puts("====== END ======");
    printf("\nOriginal N lines: %d\nNew N lines:%d\n", originallinec, newlinec);
  }

  fclose(f_input);
  fclose(f_output);

  return 0;
}

int wrap_dumbhyph(char* filepath, char* ovalue, long MAXLINELEN, int vflag) {
  open_files(filepath, ovalue);

  int originallinec = 0, newlinec = 0;

  for (; fgets(readbuffer, 2048, f_input); originallinec++) {
    // printf("===== Line %d =====\n", i);

    int linelen = 0;
    int linewordc = 0;

    for (char* seg = strtok(readbuffer, " "); seg != NULL;
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

      // If current line length + next word length + 1 (space) <= MAXLINELEN,
      // print the word preceded by a space. No space if this is the
      // first word of the line. Line word count increases by 1, and
      // line length increases by word length, plus 1 if space is
      // added. Else, a new line is added and both counters are
      // reset.
      if (linelen + wordlen + 1 <= MAXLINELEN) {
        fprintf(f_output, " %s", seg);
        linelen += wordlen + 1;
        linewordc++;
      } else {
        // N of characters available for current line, minus 2 for the hyphen
        // and the space.
        int remaining = MAXLINELEN - linelen - 2;
        if (remaining <= 0) {
          // If no character can be filled, flush the full word to the next
          // line.
          fprintf(f_output, "\n%s", seg);
          linelen = wordlen;
        } else {
          // Else, split the word into two parts.
          char* part1 = malloc((remaining + 1) * sizeof(char));
          for (int i = 0; i < remaining; i++)
            part1[i] = seg[i];
          part1[remaining] = '\0';
          char* part2 = malloc((wordlen - remaining + 1) * sizeof(char));
          for (int i = 0; i < wordlen - remaining; i++)
            part2[i] = seg[i + remaining];
          part2[wordlen - remaining] = '\0';
          // The first part of the word will be printed, preceded by a space,
          // and followed by a hyphen.
          fprintf(f_output, " %s-", part1);
          // The second part will be printed on a new line.
          fprintf(f_output, "\n%s", part2);
          linelen = wordlen - remaining;
        }
        newlinec++;
        linewordc = 1;
      }
    }

    // printf("\n");
  }
  printf("\n");

  if (vflag == 1) {
    puts("====== END ======");
    printf("\nOriginal N lines: %d\nNew N lines:%d\n", originallinec, newlinec);
  }

  fclose(f_input);
  fclose(f_output);

  return 0;
}

static void open_files(char* input, char* output) {
  f_input = fopen(input, "r");

  if (f_input == NULL) {
    printf("Failed to open %s:\n"
           "File does not exist or you don't have read permissions.\n",
           input);
    exit(EXIT_FAILURE);
  }

  readbuffer = malloc(2048 * sizeof(char));

  f_output = stdout;
  if (output != NULL) {
    f_output = fopen(output, "w");
  }
}
