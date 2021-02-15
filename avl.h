#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "stdlib.h"
#include "stdbool.h"
#include "simple_functions.h"
#include "list.h"

typedef struct AVLTreeNode {
  struct AVLTreeNode *left;
  struct AVLTreeNode *right;
  void *e;
  unsigned int height;
} avl_tree_node;

typedef struct AVLTree {
  avl_tree_node *root;
  unsigned int height;
  unsigned int len;
} avl_tree;

typedef struct AVLTreeSearchResult {
  bool found;
  void * e;
} search_result;

static inline avl_tree_node *avl_tree_new_node(void* e) {
  avl_tree_node *n = (avl_tree_node*) malloc(sizeof(avl_tree_node));
  n->left = n->right = NULL;
  n->e = e;
  n->height = 1;
  return n;
}

static inline avl_tree *avl_tree_new() {
  avl_tree *t = (avl_tree*) malloc(sizeof(avl_tree));
  t->root = NULL;
  t->height = 0;
  t->len = 0;
  return t;
}

/* Destroy a avl_tree and all elements inside */
// Used to free nodes in a avl_tree
void avl_tree_free_subnodes(avl_tree_node *n, void (*del) (void *e));
// Not exactly meant to be called directly, main logic behind destroying the avl_tree
static inline void avl_tree_del(avl_tree *t, void (*del) (void *e)) {
  // Set initial starting node
  if (t->root != NULL)
    avl_tree_free_subnodes(t->root, del);
  // Walk down the left side
  free(t);
}
// Will not remove items that are stack allocated
static inline void avl_tree_del_stack(avl_tree *t) { avl_tree_del(t, stack_del); }
// Will free elements before removing the nodes and vec
static inline void avl_tree_del_heap(avl_tree *t) { avl_tree_del(t, heap_del); }

// Get height of node
static inline unsigned int avl_tree_node_height(avl_tree_node* n) {
  return n == NULL ? 0 : n->height;
}

// Get height of the avl_tree
unsigned int avl_tree_height(const avl_tree *t);

// Check to see if the node is a leaf
static inline bool avl_tree_is_leaf(avl_tree_node *n) { return (n->left == NULL && n->right == NULL); }
// Check to see if the avl_tree_is_empty
static inline bool avl_tree_is_empty(avl_tree *t) { return t->root == NULL; }

// Recursive function to help get max value of avl_tree
void *avl_tree_max_from(avl_tree_node *node);
// Get max value of node in avl_tree
static inline void *avl_tree_max(avl_tree *t) {
  return avl_tree_max_from(t->root);
}

// Recursive function to help get min value of avl_tree
void *avl_tree_min_from(avl_tree_node *node);
// Get min value of node in avl_tree
static inline void *avl_tree_min(avl_tree *t) {
  return avl_tree_min_from(t->root);
}

/* avl_tree rotations */
avl_tree_node *avl_tree_left_rotate(avl_tree_node *r);
avl_tree_node *avl_tree_right_rotate(avl_tree_node *r);

// Get Balance factor of node n
int avl_tree_get_node_balance(avl_tree_node *n);

// Recursive function to avl_tree_insert_from a key in the avl tree rooted
// with node and returns the new root of the avl tree.
avl_tree_node* avl_tree_insert_from(
  avl_tree *t,
  avl_tree_node* node,
  void *e,
  int (*cmp) (const void *a, const void *b)
);
// Simple wrapper for recursive function
static inline void avl_tree_insert_with(avl_tree *t, void *e, int (*cmp) (const void *a, const void *b)) {
  t->root = avl_tree_insert_from(t, t->root, e, cmp);
  ++t->len;
  t->height = avl_tree_height(t);
}

// Simple wrapper for recursive function
static inline void avl_tree_insert(avl_tree *t, void *e) {
  avl_tree_insert_with(t, e, simple_cmp);
}

// Recursive function to delete a node with given e
// from avl tree with given node. It returns node of
// the modified avl tree.
avl_tree_node* avl_tree_remove_from(
  avl_tree *t,
  avl_tree_node* node,
  void *e,
  int (*cmp) (const void *a, const void *b),
  void (*del) (void *e)
);

// Simple wrapper for recursive function
static inline void avl_tree_remove_with(
  avl_tree *t,
  void *e,
  int (*cmp) (const void *a, const void *b),
  void (*del) (void *e)
) {
  t->root = avl_tree_remove_from(t, t->root, e, cmp, del);
  --t->len;
  t->height = avl_tree_height(t);
}

// Simple wrapper for removing stack allocated objects
static void avl_tree_remove(avl_tree *t, void *e) {
  avl_tree_remove_with(t, e, simple_cmp, stack_del);
}

// Find based on certain function and height
search_result avl_tree_get_from(
  avl_tree *t,
  avl_tree_node *n,
  const void *e,
  int (*cmp) (const void *a, const void *b)
);

// Wrapper for before but you can specify search function
static inline search_result avl_tree_get_with(
  avl_tree *t,
  const void *e,
  int (*cmp) (const void *a, const void *b)
) {
  return avl_tree_get_from(t, t->root, e, cmp);
}

// Avl tree searching
static inline search_result avl_tree_get(avl_tree *t, const void *e) {
  return avl_tree_get_with(t, e, simple_cmp);
}

// A utility function to print preorder traversal of the avl_tree.
// The function also prints height of every node
void avl_tree_printr(avl_tree_node *node, unsigned int offset, char *format);
static inline void avl_tree_print(avl_tree *t, char *format) {
  // Print 4 spaces between nodes
  avl_tree_printr(t->root, t->height*4, format);
}
static inline void avl_tree_println(avl_tree *t, char *format) {
  // Print 4 spaces between nodes
  avl_tree_printr(t->root, t->height*4, format);
  printf("\n");
}

// Converts the tree into a sorted list
list *avl_tree_to_list(const avl_tree *t);

#endif
