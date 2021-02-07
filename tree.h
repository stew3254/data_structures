#ifndef TREE_H
#define TREE_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct TreeNode {
  struct TreeNode *left;
  struct TreeNode *right;
  void *e;
  unsigned int height;
} tree_node;

typedef struct AVLTree {
  tree_node *root;
  unsigned int height;
} tree;

static inline tree_node *tree_new_node(void* e) {
  tree_node *n = (tree_node*) malloc(sizeof(tree_node));
  n->left = n->right = NULL;
  n->e = e;
  n->height = 1;
  return n;
}

static inline tree *tree_new() {
  tree *t = (tree*) malloc(sizeof(tree));
  t->root = NULL;
  t->height = 0;
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

static inline unsigned int tree_node_height(tree_node* n) {
  return n == NULL ? 0 : n->height;
};

static inline bool tree_is_leaf(tree_node *n) { return (n->left == NULL && n->right == NULL); }
static inline bool tree_is_empty(tree *t) { return tree_is_leaf(t->root); }

void tree_free_subnodes(tree_node *n, bool is_heap);

void *tree_max_from(tree_node *node);
static inline void *tree_max(tree *t) {
  return tree_max_from(t->root);
}
void *tree_min_from(tree_node *node);
static inline void *tree_min(tree *t) {
  return tree_min_from(t->root);
}

/* Tree rotations */
tree_node *tree_right_rotate(tree *t, tree_node *y);
tree_node *tree_left_rotate(tree *t, tree_node *x);

// Get Balance factor of node n
int tree_get_node_balance(tree_node *n);

// Recursive function to tree_insert_from a key in the subtree rooted
// with node and returns the new root of the subtree.
tree_node* tree_insert_from(tree *t, tree_node* node, void *e);
//Simple wrapper for recursive function
static void tree_insert(tree *t, void *e) {
  t->root = tree_insert_from(t, t->root, e);
}

// Recursive function to delete a node with given e
// from subtree with given node. It returns node of
// the modified subtree.
tree_node* tree_remove_from(tree *t, tree_node* node, void *e);
//Simple wrapper for recursive function
static void tree_remove(tree *t, void *e) {
  t->root = tree_remove_from(t, t->root, e);
}

// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void tree_printr(tree_node *node, unsigned int offset, char *format);
static inline void tree_print(tree *t, char *format) {
  //  Print 4 spaces between nodes
  tree_printr(t->root, t->height*4, format);
}
static inline void tree_println(tree *t, char *format) {
  //  Print 4 spaces between nodes
  tree_printr(t->root, t->height*4, format);
  printf("\n");
}

#endif
