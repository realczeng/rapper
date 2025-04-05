#include "trie.h"

extern TrieNode* trie_node_create() {
  TrieNode* node = malloc(sizeof(TrieNode));
  for (int i = 0; i < TREE_CHILDREN; i++)
    node->children[i] = NULL;
  node->points = NULL;
  node->points_len = 0;
  return node;
}

extern inline int char_index(char c) {
  if (c >= 'a' && c <= 'z')
    return c - 'a';
  if (c == '.')
    return TREE_CHILDREN - 1;
  exit(EXIT_FAILURE);
  return 0;
}

extern void trie_insert(TrieNode* t, const char* patletters, int* patnums,
                        const int patnum_len) {
  // TrieNode* t = trie_root;
  for (int i = 0; patletters[i] != '\0'; i++) {
    if (t->children[char_index(patletters[i])] == NULL)
      t->children[char_index(patletters[i])] = trie_node_create();
    t = t->children[char_index(patletters[i])];
  }
  t->points = patnums;
  // t->points = malloc(patnum_len * sizeof(int));
  // memcpy(t->points, patnums, patnum_len * sizeof(int));
  t->points_len = patnum_len;
}

extern void trie_node_free(TrieNode* node) {
  if (node == NULL)
    return;

  for (int i = 0; i < TREE_CHILDREN; i++)
    if (node->children[i] != NULL)
      trie_node_free(node->children[i]);

  if (node->points != NULL)
    free(node->points);

  free(node);
}