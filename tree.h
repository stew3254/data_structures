#ifndef TREE_H
#define TREE_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct TreeNode {
  struct TreeNode *left;
  struct TreeNode *right;
  void *e;
} tree_node;

typedef struct Tree {
  tree_node *root;
  int size;
  int index;
} tree;

static inline bool tree_is_leaf(tree_node *n) {return (n->left == NULL && n->right == NULL);}
static inline bool tree_is_empty(tree *t) {return (t->root->left == NULL && t->root->right == NULL);}
static inline int tree_size(tree *t) {return t->size;}

static inline tree_node *tree_new_node(void* e) {
  tree_node *n = (tree_node*) malloc(sizeof(tree_node));
  n->left = NULL;
  n->right = NULL;
  n->e = e;
  return n;
}

static inline tree *tree_new() {
  tree *t = (tree*) malloc(sizeof(tree));
  t->root = tree_new_node(0);
  t->size = 0;
  return t;
}

#endif
