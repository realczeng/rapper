#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patterns.h"
#include "trie.h"

#define BUF_SIZE 65536
#define SINGLE_PATTERN_SIZE 16
#define PATTERN_BUF_SIZE 128
#define PATTERN_PADDING 2 // This needs to be at least 1 for \0

typedef struct Hyphenator Hyphenator;
extern Hyphenator* Hyphenator_init(char* lang);
extern char* Hyphenator_hyphenate(Hyphenator* hyphenator, char* orig_lwr);
