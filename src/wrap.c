#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wrap_simple(char *filepath, char *ovalue, long MAXLINELEN, int vflag) {
  FILE *f_input = fopen(filepath, "r");

  if (f_input == NULL) {
    printf("Failed to open %s:\n"
           "File does not exist or you don't have read permissions.\n",
           filepath);
    return 1;
  }

  char *readbuffer = malloc(2048 * sizeof(char));

  FILE *f_output = stdout;
  if (ovalue != NULL) {
    f_output = fopen(ovalue, "w");
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

  if (vflag == 1) {
    puts("====== END ======");
    printf("\nOriginal N lines: %d\nNew N lines:%d\n", originallinec, newlinec);
  }

  fclose(f_input);
  fclose(f_output);

  return 0;
}
