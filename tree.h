#ifndef TREE_H
#define TREE_H

#include "stdlib.h"
#include "stdbool.h"

//Simple binary tree
typedef struct TreeNode {
  struct TreeNode *left;
  struct TreeNode *right;
  void *e;
} tree_node;

typedef struct Tree {
  tree_node *root;
  unsigned int len;
} tree;

static inline tree_node *tree_new_node(void* e) {
  tree_node *n = (tree_node*) malloc(sizeof(tree_node));
  n->left = n->right = NULL;
  n->e = e;
  return n;
}

static inline tree *tree_new() {
  tree *t = (tree*) malloc(sizeof(tree));
  t->root = tree_new_node(NULL);
  t->len = 0;
  return t;
}


/* Destroy a tree and all elements inside */
//Used to free nodes in a tree
void tree_free_subnodes(tree_node *n, bool is_heap);
//Not exactly meant to be called directly, main logic behind destroying the tree
static inline void tree_del(tree *t, bool is_heap) {
  //Set initial starting node
  tree_free_subnodes(t->root, is_heap);
  //Walk down the left side
  free(t);
}
//Will not remove items that are stack allocated
static inline void tree_del_stack(tree *t) { tree_del(t, false); }
//Will free elements before removing the nodes and vec
static inline void tree_del_heap(tree *t) { tree_del(t, true); }

// Check to see if the node is a leaf
static inline bool tree_is_leaf(tree_node *n) { return (n->left == NULL && n->right == NULL); }
// Check to see if the avl_tree_is_empty
static inline bool avl_tree_is_empty(tree *t) { return t->root == NULL; }

#endif