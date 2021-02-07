#include <stdio.h>
#include "simple_functions.h"
#include "tree.h"

//Used to free nodes in a avl_tree
void tree_free_subnodes(tree_node *n, bool is_heap) {
  //Free left hand side
  if (n->left != NULL) {
    tree_free_subnodes(n->left, is_heap);
  }
  //Free right hand side
  if (n->right != NULL) {
    tree_free_subnodes(n->right, is_heap);
  }

  //Clean up current node
  if (is_heap) {
    free(n->e);
  }
  free(n);
}
