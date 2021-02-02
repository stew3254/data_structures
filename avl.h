#ifndef avl_h_
#define avl_h_

#include <iostream>
#include <string>

struct node {
	void s;		//student record
	node *left;	//a pointer to the left child
	node *right;	//a poitner to the right child
  int height;  //height in tree

  node(student s, node *left, node *right, int h) {
    this->s = s;
    this->left = left;
    this->right = right;
    this->height = h;
  }

  node(student s) {
    this->s = s;
    this->left = NULL;
    this->right = NULL;
    this->height = 1;
  }
};

class AVL {
private:
	node *root;		//the root node of the tree

  //Helper for searching whole tree for something other than node id
  template <typename T>
    T search(node *n, float &accum, int order, T f (node*, float&));

  //Delete entire tree efficiently
  void deleteTree(node *n);

  //Calculate height of given node
  int height(node *n);

  //Tree rotations
  node *leftRotate(node *r);
  node *rightRotate(node *r);

  //Get balance of node
  int getBalance(node *n);

  //Actual behind the scenes insertion
  node *inserter(node *n, student s);

  //Actual behind the scenes deletion
  node* deleteNode(node* n, int id);

public:
	AVL() {root = NULL;}
  ~AVL() {deleteTree(root);}

	//insert a student record into the tree
  void insert(student s) {root = inserter(root, s);}

  //delete a student record from the tree based on ID
  void deleteID(int id) {deleteNode(root, id);}

	//print the height of the current AVL tree
	void showHeight() {cout << root->height - 1 << endl;}

  //print all records, sorted by student ID. This is essentially an in-order traversal of the tree.
  void printAll();

	//return a student's GPA based on his/her ID
	float GPA(int id);

	//return the max GPA of all the students
	float maxGPA();

  //Prints tree in the structure it is (but sideways. You need to tilt your head)
  void prettyPrint();
};

#endif
