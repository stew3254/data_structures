#include <functional>
#include <iostream>
#include "avl.h"

/*
 * Private
 *
 * */

//Helper for searching whole tree for something other than node id
template <typename T>
T AVL::search(node *n, float& accum, int order, T f (node*, float&)) {
  T ret;
  if (n == NULL);

  //Left Right Current
  if (order == -2) {
    if (n->left != NULL)
      search(n->left, accum, order, f);
    if (n->right != NULL)
      search(n->right, accum, order, f);
    ret = f(n, accum);
  }
  //Left Current Right
  else if (order == -1) {
    if (n->left != NULL)
      search(n->left, accum, order, f);
    ret = f(n, accum);
    if (n->right != NULL)
      search(n->right, accum, order, f);
  }
  //Current Left Right
  else if (order == 0) {
    ret = f(n, accum);
    if (n->left != NULL)
      search(n->left, accum, order, f);
    if (n->right != NULL)
      search(n->right, accum, order, f);
  }
  //Right Current Left
  else if (order == 1) {
    if (n->right != NULL)
      search(n->right, accum, order, f);
    ret = f(n, accum);
    if (n->left != NULL)
      search(n->left, accum, order, f);
  }
  //Right Left Current
  else if (order == 2) {
    if (n->right != NULL)
      search(n->right, accum, order, f);
    if (n->left != NULL)
      search(n->left, accum, order, f);
    ret = f(n, accum);
  }
  return ret;
}

//Delete entire tree efficiently
void AVL::deleteTree(node *n) {
  int (*del) (node*,float&) = [] (node *n, float &i) {delete n; return 0;};
  float dummy;
  //LRC
  search(n, dummy, -2, del);
}

//Calculate height of given node
int AVL::height(node *n) {
  if (n == NULL)
    return 0;
  return n->height;
}

//Left tree rotation
node *AVL::leftRotate(node *r) {
  node *x = r->right;
  node *y = x->left;

  //Rotate
  x->left = r;
  r->right = y;

  r->height -= 2;
  return x;
}

//Right tree rotation
node *AVL::rightRotate(node *r) {
  node *x = r->left;
  node *y = x->right;

  //Rotate
  x->right = r;
  r->left = y;

  r->height -= 2;
  return x;
}

//See if subtree is balanced
int AVL::getBalance(node *n) {
  if (n == NULL)
    return 0;
  return height(n->right) - height(n->left);
}

//Actual behind the scenes insertion
node *AVL::inserter(node *n, student s) {
  if (n == NULL)
    return new node(s);

  //Recursively perform insertion
  if (s.id < n->s.id)
    n->left = inserter(n->left, s);
  else if (s.id > n->s.id)
    n->right = inserter(n->right, s);
  else
    return n;

  //Recalculate height of node
  n->height = max(height(n->left), height(n->right)) + 1;

  //Recalculate balance of the tree
  int balance = getBalance(n);

  //Left Left Case
  if (balance < -1 && s.id < n->left->s.id)
    return rightRotate(n);

  //Right Right Case
  if (balance > 1 && s.id > n->right->s.id)
    return leftRotate(n);

  //Left Right Case
  if (balance < -1 && s.id > n->left->s.id) {
    n->left = leftRotate(n->left);
    return rightRotate(n);
  }

  //Right Left Case
  if (balance > 1 && s.id < n->right->s.id) {
    n->right = rightRotate(n->right);
    return leftRotate(n);
  }

  return n;
}


//Actual behind the scenes deletion
node* AVL::deleteNode(node* n, int id) {
  if (n == NULL)
    return n;
  //Traverse tree
  if (id < n->s.id)
    n->left = deleteNode(n->left, id);
  else if(id > n->s.id)
    n->right = deleteNode(n->right, id);
  else {
    //Node without both children
    if((n->left == NULL) || (n->right == NULL)) {
      //Check to see if assignment of n->left is null, if it is, do n->right
      node *temp = n->left ? n->left : n->right;

      //No child case
      if (temp == NULL) {
        //Free node
        temp = n;
        n = NULL;
      }
      else
        //Set contents of temp in the contents of n, not same as pointer swap
        *n = *temp;
      delete temp;
    }
    else {
      //Find min value of right subtree
      node* temp;
      for (temp = n->right; temp->left != NULL; temp = temp->left);

      //Copy the successor's data here
      n->s = temp->s;

      //Delete the successor
      n->right = deleteNode(n->right, temp->s.id);
    }
  }

  // If the tree had only one node
  if (n == NULL)
    return n;

  //Update height
  n->height = max(height(n->left), height(n->right)) + 1;

  //Get the balance factor
  int balance = getBalance(n);

  //Rebalance nodes
  //Left Left Case
  if (balance < -1 && getBalance(n->left) <= 0)
    return rightRotate(n);

  //Left Right Case
  if (balance < -1 && getBalance(n->left) > 0) {
    n->left = leftRotate(n->left);
    return rightRotate(n);
  }

  // Right Right Case
  if (balance > 1 && getBalance(n->right) >= 0)
    return leftRotate(n);

  // Right Left Case
  if (balance > 1 && getBalance(n->right) < 0) {
    n->right = rightRotate(n->right);
    return leftRotate(n);
  }

  return n;
}

/*
 * Public
 *
 * */

//return a student's GPA based on his/her ID
float AVL::GPA(int id) {
  node *n = root;
  while (n != NULL && n->s.id != id)
    (n->s.id < id) ?  n = n->right : n = n->left;
  if (n == NULL)
    return -1.0;
  return n->s.gpa;
}

//return the max GPA of all the students
float AVL::maxGPA() {
  float max = 0;
  float (*mg) (node*,float&) = [] (node *n, float &max) {
    if (n->s.gpa > max)
      max = n->s.gpa;
    return max;
  };
  //LCR
  search(root, max, -1, mg);
  return max;
}

//print all records, sorted by student ID. This is essentially an in-order traversal of the tree.
void AVL::printAll() {
  int (*p) (node*,float&) = [] (node *n,float &i) {
    cout << "ID: " << n->s.id << ";\tHeight: " << n->height - 1 << endl;
    return 0;
  };
  float dummy;
  //LCR
  search(root, dummy, -1, p);
}

//Prints tree out sideways (works best on small trees for debugging)
void AVL::prettyPrint() {
  float maxHeight = (float) root->height;
  int (*p) (node*,float&) = [] (node *n, float &maxHeight) {
    for (int i = n->height; i < (int) maxHeight; ++i)
      cout << "  ";
    cout << n->s.id << endl;
    return 0;
  };
  //RCL
  search(root, maxHeight, 1, p);
}
