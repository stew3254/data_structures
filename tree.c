/*
 * NOTE: Much of this code was taking from geeksforgeeks.org and then
 * adapted to fit my paradigm. I do not take credit for this algorithm's
 * implementation, only for the structuring of the functions. The only thing
 * I wrote myself thus far is the BFS style avl_tree print
 */
#include <stdio.h>
#include "simple_functions.h"
#include "tree.h"

//Used to free nodes in a avl_tree
void tree_free_subnodes(avl_tree_node *n, bool is_heap) {
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

// Find the maximum value in the avl_tree
void *tree_max_from(avl_tree_node *node) {
  avl_tree_node* current = node;

  /* loop down to find the leftmost leaf */
  while (current->right != NULL)
    current = current->right;

  return current->e;
}

// Find the minimum value in the avl_tree
void *tree_min_from(avl_tree_node *node) {
  avl_tree_node* current = node;

  /* loop down to find the leftmost leaf */
  while (current->left != NULL)
    current = current->left;

  return current->e;
}

// Get Balance factor of node n
int tree_get_node_balance(avl_tree_node *n) {
  if (n == NULL)
    return 0;
  return (int) tree_node_height(n->right) - (int) tree_node_height(n->left);
}


// A utility function to left rotate subtree rooted with r
// See the diagram given above.
avl_tree_node *tree_left_rotate(avl_tree *t, avl_tree_node *r) {
  avl_tree_node *x = r->right;
  avl_tree_node *y = x->left;

  // Perform rotation
  x->left = r;
  r->right = y;

  //  Update heights
  // y->height = max((void *) tree_node_height(y->left), (void *) tree_node_height(y->right)) + 1;
  // x->height = max((void *) tree_node_height(x->left), (void *) tree_node_height(x->right)) + 1;
  r->height -= 2;

  // Return new root
  return x;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
avl_tree_node *tree_right_rotate(avl_tree *t, avl_tree_node *r) {
  avl_tree_node *x = r->left;
  avl_tree_node *y = x->right;

  // Perform rotation
  x->right = r;
  r->left = y;

  // Update heights
  // y->height = max((void *) tree_node_height(y->left), (void *) tree_node_height(y->right)) + 1;
  // x->height = max((void *) tree_node_height(x->left), (void *) tree_node_height(x->right)) + 1;
  r->height -= 2;

  // Return new root
  return x;
}

// Recursive function to tree_insert_from a key in the subtree rooted
// with node and returns the new root of the subtree.
avl_tree_node* tree_insert_from(avl_tree *t, avl_tree_node* node, void *e) {

  /* 1.  Perform the normal BST insertion */
  if (node == NULL)
    return tree_new_node(e);

  //Insert left in the avl_tree
  if (e < node->e)
    node->left  = tree_insert_from(t, node->left, e);
  //Insert right in the avl_tree
  else if (e > node->e)
    node->right = tree_insert_from(t, node->right, e);
  //We cannot have duplicates
  else // Equal keys are not allowed in BST
    return node;

  /* 2. Update height of this ancestor node */
  node->height = max(
      (void *) tree_node_height(node->left),
      (void *) tree_node_height(node->right)
  ) + 1;

  /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
  int balance = tree_get_node_balance(node);

  // If this node becomes unbalanced, then
  // there are 4 cases

  // Left Left Case
  if (balance < -1 && e < node->left->e)
    return tree_right_rotate(t, node);

  // Right Right Case
  if (balance > 1 && e > node->right->e)
    return tree_left_rotate(t, node);

  // Left Right Case
  if (balance < -1 && e > node->left->e) {
    node->left = tree_left_rotate(t, node->left);
    return tree_right_rotate(t, node);
  }

  // Right Left Case
  if (balance > 1 && e < node->right->e) {
    node->right = tree_right_rotate(t, node->right);
    return tree_left_rotate(t, node);
  }

  /* return the (unchanged) node pointer */
  return node;
}

// Recursive function to delete a node with given e
// from subtree with given node. It returns node of
// the modified subtree.
avl_tree_node* tree_remove_from(avl_tree *t, avl_tree_node* node, void *e) {
  // STEP 1: PERFORM STANDARD BST DELETE
  if (node == NULL)
    return node;

  // If the e to be deleted is smaller than the
  // node's e, then it lies in left subtree
  if (e < node->e ) {
    node->left = tree_remove_from(t, node->left, e);
  } else if(e > node->e ) {
    // If the e to be deleted is greater than the
    // node's e, then it lies in right subtree
    node->right = tree_remove_from(t, node->right, e);
  } else {
    // if e is same as node's e, then This is
    // the node to be deleted
    // node with only one child or no child
    if ((node->left == NULL) || (node->right == NULL)) {
      avl_tree_node *temp = node->left ? node->left : node->right;

      // No child case
      if (temp == NULL) {
        temp = node;
        node = NULL;
      } else {
        // One child case
        // Copy the contents of the non-empty child
        *node = *temp;
      }
      free(temp);
    } else {
      // node with two children: Get the inorder
      // successor (smallest in the right subtree)
      // Copy the inorder successor's data to this node
      node->e = tree_min_from(node->right);

      // Delete the inorder successor
      node->right = tree_remove_from(t, node->right, node->e);
    }
  }

  // If the avl_tree had only one node then return
  if (node == NULL)
    return node;

  // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
  node->height = max(
      (void *) tree_node_height(node->left),
      (void *) tree_node_height(node->right)
  ) + 1;

  // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
  // check whether this node became unbalanced)
  int balance = tree_get_node_balance(node);

  // If this node becomes unbalanced, then there are 4 cases

  // Left Left Case
  if (balance < -1 && tree_get_node_balance(node->left) <= 0)
    return tree_right_rotate(t, node);

  // Left Right Case
  if (balance < -1 && tree_get_node_balance(node->left) > 0) {
    node->left =  tree_left_rotate(t, node->left);
    return tree_right_rotate(t, node);
  }

  // Right Right Case
  if (balance > 1 && tree_get_node_balance(node->right) >= 0)
    return tree_left_rotate(t, node);

  // Right Left Case
  if (balance > 1 && tree_get_node_balance(node->right) < 0) {
    node->right = tree_right_rotate(t, node->right);
    return tree_left_rotate(t, node);
  }

  return node;
}

// A utility function to print preorder traversal of the avl_tree.
// The function also prints height of every node
void tree_printr(avl_tree_node *node, unsigned int offset, char *format) {
  if(node != NULL) {
    printf(format, node->e);
    tree_printr(node->left, offset - 4, format);
    tree_printr(node->right, offset - 4, format);
  }
}
