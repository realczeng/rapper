#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 65536
#define SINGLE_PATTERN_SIZE 16
#define PATTERN_BUF_SIZE 128
#define PATTERN_PADDING 2 // This needs to be at least 1 for \0

typedef struct Patterns {
  int n_patterns;
  char* patterns;
} Patterns;

extern Patterns* patterns_init(char* fname);
extern int patterns_set(Patterns* pat, int index, const char* pattern);
extern char* patterns_get(Patterns* pat, int index);
extern int patterns_free(Patterns* pat);

extern char* patterns_letters(const char* pat);
extern int* patterns_nums(const char* pat, int* patnum_len);
