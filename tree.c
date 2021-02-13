#include <stdio.h>
#include "simple_functions.h"
#include "tree.h"

// Used to free nodes in a avl_tree
void tree_free_subnodes(tree_node *n, void (*del) (void *e)) {
  // Free left hand side
  if (n->left != NULL) {
    tree_free_subnodes(n->left, del);
  }
  // Free right hand side
  if (n->right != NULL) {
    tree_free_subnodes(n->right, del);
  }

  // Clean up current node
  del(n->e);
  free(n);
}
