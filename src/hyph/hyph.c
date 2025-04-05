#include "hyph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Hyphenator {
  char* lang;
  TrieNode* patterns;
} Hyphenator;

extern Hyphenator* Hyphenator_init(char* lang) {
  Hyphenator* hyph = malloc(sizeof(Hyphenator));
  // Return NULL if malloc fails.
  if (hyph == NULL)
    return NULL;

  // Currently, only English is supported.
  hyph->lang = malloc(8 * sizeof(char));
  strcpy(hyph->lang, "en");
  Patterns* patterns = patterns_init("patterns_en.txt");

  // Load English-language hyphenation patterns.
  TrieNode* trie = trie_node_create();
  for (int i = 0; i < patterns->n_patterns; i++) {
    char* plets = patterns_letters(patterns_get(patterns, i));
    int res_len = 0;
    int* res_nums = patterns_nums(patterns_get(patterns, i), &res_len);

    trie_insert(trie, plets, res_nums, res_len);
  }
  hyph->patterns = trie;
  patterns_free(patterns);
  return hyph;
}

extern char* Hyphenator_hyphenate(Hyphenator* hyphenator, char* orig_lwr) {

  int orig_len = strlen(orig_lwr);

  char* res = calloc(2 * orig_len + 2, sizeof(char));
  if (res == NULL)
    exit(EXIT_FAILURE);
  // Words with fewer than four letters are not hyphenated.
  if (orig_len <= 4) {
    strcpy(res, orig_lwr);
    return res;
  }

  // TODO: implement exception handling
  if (0)
    ;
  else {
    char* work = calloc(orig_len + 3, sizeof(char));
    work[0] = '.';
    strcpy(work + 1, orig_lwr);
    work[orig_len + 1] = '.';
    // CALLOC already set work[orig_len + 2] to '\0'.

    int work_len = orig_len + 2;
    int* points = calloc(work_len + 1, sizeof(int));
    for (int i = 0; i < work_len; i++) {
      TrieNode* t = hyphenator->patterns;
      // for c in work[i:]:
      for (char* c = work + i; *c != '\0'; c++) {
        // if c in t:
        if (t->children[char_index(*c)] != NULL) {
          // t = t[c]
          t = t->children[char_index(*c)];
          // if None in t:
          if (t->points != NULL) {
            // p = t[None]
            int* p = t->points;
            // for j in range(len(p)):
            for (int j = 0; j < t->points_len; j++) {
              points[i + j] = points[i + j] > p[j] ? points[i + j] : p[j];
            }
          }
        } else
          break;
      }
    }

    // TODO: No hyphens in the first two chars or the last two.
    // points[1] = points[2] = points[-2] = points[-3] = 0;
    char** pieces = malloc(orig_len * sizeof(char*));
    int pieces_index = 0;
    pieces[pieces_index] = calloc(orig_len + 1, sizeof(char));
    int char_index = 0;

    for (int i = 0; i < orig_len; i++) {
      pieces[pieces_index][char_index++] = orig_lwr[i];
      printf("%c ", orig_lwr[i]);
      if (points[i + 2] % 2 != 0) {
        pieces_index++;
        pieces[pieces_index] =
            calloc(orig_len + 1, sizeof(char)); // New segment
        char_index = 0;
      }
    }
    printf("\n");
    for (int i = 0; i < orig_len; i++) {
      printf("%d ", points[i + 2]);
    }
    printf("\n");
    int res_c = 0;
    for (int i = 0; i < pieces_index; i++) {
      // int current_piece_len = strlen(pieces[i]);
      // strcpy(res + res_c, pieces[i]);
      // res[current_piece_len] = '-';
      // res_c += current_piece_len;
      for (int j = 0; pieces[i][j] != '\0'; j++) {
        res[res_c++] = pieces[i][j];
      }
      res[res_c++] = '-';
      printf("%s-", pieces[i]);
    }
    for (int j = 0; pieces[pieces_index][j] != '\0'; j++) {
      res[res_c++] = pieces[pieces_index][j];
    }
    printf("%s\n", pieces[pieces_index]);
    free(points);
    free(pieces);
    free(work);
  }

  return res;
}

int Hyphenator_free(Hyphenator* hyph) {
  free(hyph->lang);
  trie_node_free(hyph->patterns);
  return 0;
}

int main(int argc, char** argv) {
  Hyphenator* hyph = Hyphenator_init("en");

  if (argv[1] != NULL)
    printf("%s\n", Hyphenator_hyphenate(hyph, argv[1]));
  else
    printf("%s\n", Hyphenator_hyphenate(hyph, "unbelievably"));

  Hyphenator_free(hyph);
}
