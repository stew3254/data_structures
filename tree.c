/*
 * NOTE: Much of this code was taking from geeksforgeeks.org and then
 * adapted to fit my paradigm. I do not take credit for this algorithm's
 * implementation, only for the structuring of the functions. The only thing
 * I wrote myself thus far is the BFS style tree print
 */
#include <stdio.h>
#include "simple_functions.h"
#include "tree.h"

//Used to free nodes in a tree
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
    free(n);
  }
}

// Find the maximum value in the tree
void *tree_max_from(tree_node *node) {
  tree_node* current = node;

  /* loop down to find the leftmost leaf */
  while (current->right != NULL)
    current = current->right;

  return current->e;
}

// Find the minimum value in the tree
void *tree_min_from(tree_node *node) {
  tree_node* current = node;

  /* loop down to find the leftmost leaf */
  while (current->left != NULL)
    current = current->left;

  return current->e;
}

// Get Balance factor of node n
int tree_get_node_balance(tree_node *n) {
  if (n == NULL)
    return 0;
  return (int) tree_node_height(n->left) - (int) tree_node_height(n->right);
}


// A utility function to right rotate subtree rooted with y
// See the diagram given above.
tree_node *tree_right_rotate(tree *t, tree_node *y) {
  tree_node *x = y->left;
  tree_node *T2 = x->right;

  // Perform rotation
  x->right = y;
  y->left = T2;

  // Update heights
  y->height = max((void *) tree_node_height(y->left), (void *) tree_node_height(y->right)) + 1;
  x->height = max((void *) tree_node_height(x->left), (void *) tree_node_height(x->right)) + 1;

  //Update tree height
  t->height = max((void *) t->height, (void *) tree_node_height(y));

  // Return new root
  return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
tree_node *tree_left_rotate(tree *t, tree_node *x) {
  tree_node *y = x->right;
  tree_node *T2 = y->left;

  // Perform rotation
  y->left = x;
  x->right = T2;

  //  Update heights
  y->height = max((void *) tree_node_height(y->left), (void *) tree_node_height(y->right)) + 1;
  x->height = max((void *) tree_node_height(x->left), (void *) tree_node_height(x->right)) + 1;

  //Update tree height
  t->height = max((void *) t->height, (void *) tree_node_height(x));

  // Return new root
  return y;
}
// Recursive function to tree_insert_from a key in the subtree rooted
// with node and returns the new root of the subtree.
tree_node* tree_insert_from(tree *t, tree_node* node, void *e) {

  /* 1.  Perform the normal BST insertion */
  if (node == NULL)
    return(tree_new_node(e));

  //Insert left in the tree
  if (e < node->e)
    node->left  = tree_insert_from(t, node->left, e);
  //Insert right in the tree
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
  if (balance > 1 && e < node->left->e)
    return tree_right_rotate(t, node);

  // Right Right Case
  if (balance < -1 && e > node->right->e)
    return tree_left_rotate(t, node);

  // Left Right Case
  if (balance > 1 && e > node->left->e) {
    node->left = tree_left_rotate(t, node->left);
    return tree_right_rotate(t, node);
  }

  // Right Left Case
  if (balance < -1 && e < node->right->e) {
    node->right = tree_right_rotate(t, node->right);
    return tree_left_rotate(t, node);
  }

  /* return the (unchanged) node pointer */
  return node;
}

// Recursive function to delete a node with given e
// from subtree with given root. It returns root of
// the modified subtree.
tree_node* tree_remove_from(tree *t, tree_node* root, void *e) {
  // STEP 1: PERFORM STANDARD BST DELETE
  if (root == NULL)
    return root;

  // If the e to be deleted is smaller than the
  // root's e, then it lies in left subtree
  if (e < root->e ) {
    root->left = tree_remove_from(t, root->left, e);
  } else if(e > root->e ) {
    // If the e to be deleted is greater than the
    // root's e, then it lies in right subtree
    root->right = tree_remove_from(t, root->right, e);
  } else {
    // if e is same as root's e, then This is
    // the node to be deleted
    // node with only one child or no child
    if ((root->left == NULL) || (root->right == NULL)) {
      tree_node *temp = root->left ? root->left : root->right;

      // No child case
      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else {
        // One child case
        // Copy the contents of the non-empty child
        *root = *temp;
      }
      free(temp);
    } else {
      // node with two children: Get the inorder
      // successor (smallest in the right subtree)
      // Copy the inorder successor's data to this node
      root->e = tree_min_from(root->right);

      // Delete the inorder successor
      root->right = tree_remove_from(t, root->right, root->e);
    }
  }

  // If the tree had only one node then return
  if (root == NULL)
    return root;

  // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
  root->height = max(
      (void *) tree_node_height(root->left),
      (void *) tree_node_height(root->right)
  ) + 1;

  // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
  // check whether this node became unbalanced)
  int balance = tree_get_node_balance(root);

  // If this node becomes unbalanced, then there are 4 cases

  // Left Left Case
  if (balance > 1 && tree_get_node_balance(root->left) >= 0)
    return tree_right_rotate(t, root);

  // Left Right Case
  if (balance > 1 && tree_get_node_balance(root->left) < 0) {
    root->left =  tree_left_rotate(t, root->left);
    return tree_right_rotate(t, root);
  }

  // Right Right Case
  if (balance < -1 && tree_get_node_balance(root->right) <= 0)
    return tree_left_rotate(t, root);

  // Right Left Case
  if (balance < -1 && tree_get_node_balance(root->right) > 0) {
    root->right = tree_right_rotate(t, root->right);
    return tree_left_rotate(t, root);
  }

  return root;
}

// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void tree_printr(tree_node *node, unsigned int offset, char *format) {
  if(node != NULL) {
    printf(format, node->e);
    tree_printr(node->left, offset - 4, format);
    tree_printr(node->right, offset - 4, format);
  }
}
