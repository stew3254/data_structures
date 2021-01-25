#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vec.h"

//Initialize a new empty vec
vec *vec_new(size_t size) {
  vec *v = (vec *) malloc(sizeof(vec));
  v->capacity = 32;
  v->len = 0;
  v->data = calloc(v->capacity + 1, size);
  return v;
}
//
////Delete an existing vec
//void vec_del(vec *l, bool is_heap) {
//  //Save the start pointer
//  vec_node *start = l->head;
//  vec_node *n = l->head;
//  vec_node *temp_n = n;
//  do {
//    //Store pointer to next element
//    temp_n = n->next;
//    //Remove the element if it's on the heap
//    if (is_heap && n->e != NULL)
//      free(n->e);
//
//    //Delete n
//    free(n);
//    //Set it to the next pointer
//    n = temp_n;
//  } while (n != start);
//  free(l);
//}
//
//vec_node *vec_get_at(const vec *l, const unsigned int index) {
//  if (index >= l->len) {
//    return NULL;
//  }
//  vec_node *n;
//
//  bool isForward = true;
//  if (l->len / 2 - index < 0)
//    isForward = false;
//
//  if (isForward) {
//    n = l->head->next;
//    for (int i = 0; i < index; i++) {
//      n = n->next;
//    }
//  } else {
//    n = l->tail->prev;
//    for (int i = 0; i < index; i++) {
//      n = n->prev;
//    }
//  }
//  return n;
//}
//
////Find position of node in the vec
//int vec_get_at_pos(const vec *l, const vec_node* item) {
//  int pos = 0;
//  for (vec_node *n = l->head; n != item; n = n->next)
//    ++pos;
//  return pos;
//}
//
////Find the first element in the vec where simple comparison works
////Returns NULL if nothing was found
//vec_node *vec_find(const vec *l, const void *e) {
//  vec_node *n;
//  for (n = l->head; n->e != e || n != l->head; n = n->next);
//  if (n == l->head)
//    return NULL;
//  return n;
//}
//
////Find the first element in the vec based on comparison function for more advanced checks
////Returns NULL if nothing was found
//vec_node *vec_find_with(const vec *l, const void *e, int (*cmp)(const void *a, const void *b)) {
//  vec_node *n;
//  for (n = l->head; n != l->head; n = n->next) {
//    if (cmp(e, n->e) == 0)
//      return n;
//  }
//  return NULL;
//}
//
////Insert element after item
//void vec_insert(vec *l, vec_node *item, void *e) {
//  vec_node *new_node = vec_new_node(e);
//
//  //Shift the pointer so it points to the element before the fake tail
//  if (item == l->tail)
//    item = item->prev;
//
//  //Set back pointer to the item being inserted after
//  new_node->prev = item;
//  //Set next pointer to the item after the one we're inserting after
//  new_node->next = item->next;
//  //Set the forward pointer of item and back pointer of the item after the new node to the new node
//  item->next = new_node->next->prev = new_node;
//
//  //Bump up len
//  ++l->len;
//}
//
////Pop item
////Prone to memory leaking if try to free item twice
//void *vec_pop(vec *l, vec_node *item) {
//  void *e = item->e;
//  item->prev->next = item->next;
//  item->next->prev = item->prev;
//  free(item);
//  --l->len;
//  return e;
//}
//
////Reverses the vec in place
//void vec_rev(vec *l) {
//  vec_node *n = l->head;
//  vec_node *temp;
//
//  do {
//    //Move forward in the vec
//    temp = n->next;
//    //Flip the pointers
//    n->next = n->prev;
//    n->prev = temp;
//    //Continue to move forward
//    n = temp;
//  } while (n != l->head);
//  //Since the head and tail got reversed, change their order in the vec
//  temp = l->tail;
//  l->head = temp;
//  l->tail = n;
//}
//
////Does a deep copy of elements into a new vec. Allows you to specify how to deep copy
//vec *vec_copy_with(const vec *l, void *(copy)(const void *e)) {
//  vec *new_l = vec_new();
//  for (vec_node *n = l->head->next; n != l->tail; n = n->next) {
//    vec_push_back(new_l, copy(n->e));
//  }
//  return new_l;
//}
//
////Concatenates 2 vecs with copy function and returns a new vec
//vec *vec_concat_with(vec *l1, vec* l2, void *(copy)(const void *e)) {
//  //If either vec is empty, no need to concatenate. Just copy the other one
//  if (l1->len == 0)
//    return vec_copy_with(l2, copy);
//  if (l2->len == 0)
//    return vec_copy_with(l1, copy);
//
//  //Make a new vec without the head and tail
//  vec *new_l = (vec*) malloc(sizeof(vec));
//  //Copy the 2 vecs since these will be consumed
//  vec *new_l1 = vec_copy_with(l1, copy);
//  vec *new_l2 = vec_copy_with(l2, copy);
//
//  /* Stitch all of the nodes together */
//  //Set the head
//  new_l->head = new_l1->head;
//  //Set the tail, but also the previous pointer on the head ot the right tail
//  new_l->head->prev = new_l->tail = new_l2->tail;
//  //Set the tail's next to the right head
//  new_l->tail->next = new_l->head;
//  //Fix the last element in vec 1 to point to the first element of vec 2
//  vec_node *temp = new_l1->tail->prev;
//  new_l1->tail->prev->next = new_l2->head->next;
//  //Do the opposite
//  new_l2->head->next->prev = temp;
//
//
//  //Capture the new length
//  new_l->len = l1->len + l2->len;
//
//  //Free the unused nodes in the now consumed vecs
//  free(new_l1->tail);
//  free(new_l2->head);
//  //Free the consumed vecs
//  free(new_l1);
//  free(new_l2);
//
//  return new_l;
//}
//
////Print the contents of a vec for debugging
//void vec_print(const vec *l, const char *format) {
//  for (vec_node *n = l->head->next; n->next != l->head; n = n->next) {
//    printf(format, n->e);
//  }
//}
//
////Print the contents of a vec between nodes for debugging
//void vec_print_between(const vec *l, const vec_node *i, const vec_node *j, const char *format) {
//  //Does not flip nodes if they are backwards unlike other function
//  vec_node *n = i;
//  while(n != j) {
//    //Print the format per node
//    printf(format, n->e);
//    n = n->next;
//  }
//}
//
////Print the contents of a vec between indices for debugging
//void vec_print_between_indices(const vec *l, unsigned int i, unsigned int j, const char *format) {
//  //Flips signs if they are backwards
//  if (i > j) {
//    vec_print_between_indices(l, j, i, format);
//  }
//
//  //Get the location of the node
//  vec_node *n = vec_get_at(l, i);
//  while(i != j) {
//    //Print the format per node
//    printf(format, n->e);
//    n = n->next;
//    ++i;
//  }
//}
