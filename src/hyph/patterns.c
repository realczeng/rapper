#include "patterns.h"
#include <stdlib.h>

// extern int n_patterns;
// extern char* patterns;

static int count_lines(FILE* file) {
  char buf[BUF_SIZE];
  int counter = 0;
  while (1) {
    size_t res = fread(buf, 1, BUF_SIZE, file);
    if (ferror(file))
      return -1;

    int i;
    for (i = 0; i < res; i++)
      if (buf[i] == '\n')
        counter++;

    if (feof(file))
      break;
  }

  return counter;
}

extern int patterns_set(Patterns* pat, int index, const char* pattern) {
  char* dest = pat->patterns + index * (SINGLE_PATTERN_SIZE + PATTERN_PADDING);
  if (strlen(pat->patterns) > SINGLE_PATTERN_SIZE)
    return 1;
  strcpy(dest, pattern);
  return 0;
}

extern char* patterns_get(Patterns* pat, int index) {
  return pat->patterns + index * (SINGLE_PATTERN_SIZE + PATTERN_PADDING);
}

extern Patterns* patterns_init(char* fname) {
  FILE* f = fopen(fname, "r");
  if (f == NULL) {
    return NULL;
  }

  Patterns* patterns = malloc(sizeof(Patterns));
  if (patterns == NULL)
    return NULL;

  patterns->n_patterns = count_lines(f);

  //   n_patterns = count_lines(f);
  patterns->patterns = malloc((SINGLE_PATTERN_SIZE + PATTERN_PADDING) *
                              patterns->n_patterns * sizeof(char));
  //   patterns = malloc((SINGLE_PATTERN_SIZE + PATTERN_PADDING) * n_patterns *
  //                     sizeof(char));
  if (patterns->patterns == NULL) {
    return NULL;
  }
  rewind(f);
  char readbuffer[PATTERN_BUF_SIZE];

  for (int i = 0; fgets(readbuffer, PATTERN_BUF_SIZE, f) != NULL; i++) {
    // printf("%s", readbuffer);
    readbuffer[strcspn(readbuffer, "\n")] = '\0';
    patterns_set(patterns, i, readbuffer);
  }

  if (fclose(f) != 0)
    return NULL;

  return patterns;
}

extern char* patterns_letters(const char* pat) {
  // chars = re.sub(r'[0-9]', '', pattern)
  int orig_len = strlen(pat);
  char* res = calloc((orig_len + 1), sizeof(char));
  if (res == NULL)
    exit(EXIT_FAILURE);
  int dest_i = 0;
  for (int i = 0; i < orig_len; i++) {
    if (isalpha(pat[i]) || pat[i] == '.')
      res[dest_i++] = pat[i];
  }
  return res;
}

extern int* patterns_nums(const char* pat, int* patnum_len) {
  int len = strlen(pat);
  int* points = calloc(len, sizeof(int));
  if (!points) {
    exit(EXIT_FAILURE);
  }
  int points_count = 0, start = 0;
  for (int i = 0; i < len; i++) {
    char c = pat[i];
    if ((c >= 'a' && c <= 'z') || c == '.') {
      if (i - start > 0) {
        int token_len = i - start;
        char* token = malloc((token_len + 1) * sizeof(char));
        strncpy(token, pat + start, token_len);
        token[token_len] = '\0';
        points[points_count++] = atoi(token);
        free(token);
      } else {
        points[points_count++] = 0;
      }
      start = i + 1;
    }
  }
  if (len - start > 0) {
    int token_len = len - start;
    char* token = malloc(token_len + 1);
    if (!token) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(1);
    }
    strncpy(token, pat + start, token_len);
    token[token_len] = '\0';
    points[points_count++] = atoi(token);
    free(token);
  } else {
    // If there is an empty trailing token, add 0.
    points[points_count++] = 0;
  }
  *patnum_len = points_count;
  return points;
}

extern int patterns_free(Patterns* pat) {
  if (pat == NULL)
    return 1;
  free(pat->patterns);
  free(pat);
  return 0;
}