#include <stdlib.h>

#define TREE_CHILDREN 27

typedef struct TrieNode {
  struct TrieNode* children[TREE_CHILDREN];
  int* points;
  int points_len;
} TrieNode;

extern TrieNode* trie_node_create();
extern void trie_node_free(TrieNode* node);
extern int char_index(char c);
extern void trie_insert(TrieNode* t, const char* patletters, int* patnums,
                        const int patnum_len);