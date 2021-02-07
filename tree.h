#ifndef TREE_H
#define TREE_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct TreeNode {
  struct TreeNode *left;
  struct TreeNode *right;
  void *e;
  unsigned int height;
} avl_tree_node;

typedef struct AVLTree {
  avl_tree_node *root;
  unsigned int height;
} avl_tree;

static inline avl_tree_node *tree_new_node(void* e) {
  avl_tree_node *n = (avl_tree_node*) malloc(sizeof(avl_tree_node));
  n->left = n->right = NULL;
  n->e = e;
  n->height = 1;
  return n;
}

static inline avl_tree *tree_new() {
  avl_tree *t = (avl_tree*) malloc(sizeof(avl_tree));
  t->root = NULL;
  t->height = 0;
  return t;
}


/* Destroy a avl_tree and all elements inside */
//Used to free nodes in a avl_tree
void tree_free_subnodes(avl_tree_node *n, bool is_heap);
//Not exactly meant to be called directly, main logic behind destroying the avl_tree
static inline void tree_del(avl_tree *t, bool is_heap) {
  //Set initial starting node
  tree_free_subnodes(t->root, is_heap);
  //Walk down the left side
  free(t);
}
//Will not remove items that are stack allocated
static inline void tree_del_stack(avl_tree *t) { tree_del(t, false); }
//Will free elements before removing the nodes and vec
static inline void tree_del_heap(avl_tree *t) { tree_del(t, true); }

static inline unsigned int tree_node_height(avl_tree_node* n) {
  return n == NULL ? 0 : n->height;
};

static inline bool tree_is_leaf(avl_tree_node *n) { return (n->left == NULL && n->right == NULL); }
static inline bool tree_is_empty(avl_tree *t) { return tree_is_leaf(t->root); }

void tree_free_subnodes(avl_tree_node *n, bool is_heap);

void *tree_max_from(avl_tree_node *node);
static inline void *tree_max(avl_tree *t) {
  return tree_max_from(t->root);
}
void *tree_min_from(avl_tree_node *node);
static inline void *tree_min(avl_tree *t) {
  return tree_min_from(t->root);
}

/* Tree rotations */
avl_tree_node *tree_right_rotate(avl_tree *t, avl_tree_node *y);
avl_tree_node *tree_left_rotate(avl_tree *t, avl_tree_node *x);

// Get Balance factor of node n
int tree_get_node_balance(avl_tree_node *n);

// Recursive function to tree_insert_from a key in the subtree rooted
// with node and returns the new root of the subtree.
avl_tree_node* tree_insert_from(avl_tree *t, avl_tree_node* node, void *e);
//Simple wrapper for recursive function
static void tree_insert(avl_tree *t, void *e) {
  t->root = tree_insert_from(t, t->root, e);
}

// Recursive function to delete a node with given e
// from subtree with given node. It returns node of
// the modified subtree.
avl_tree_node* tree_remove_from(avl_tree *t, avl_tree_node* node, void *e);
//Simple wrapper for recursive function
static void tree_remove(avl_tree *t, void *e) {
  t->root = tree_remove_from(t, t->root, e);
}

// A utility function to print preorder traversal of the avl_tree.
// The function also prints height of every node
void tree_printr(avl_tree_node *node, unsigned int offset, char *format);
static inline void tree_print(avl_tree *t, char *format) {
  //  Print 4 spaces between nodes
  tree_printr(t->root, t->height*4, format);
}
static inline void tree_println(avl_tree *t, char *format) {
  //  Print 4 spaces between nodes
  tree_printr(t->root, t->height*4, format);
  printf("\n");
}

#endif
