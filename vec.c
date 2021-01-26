#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vec.h"

//Initialize a new empty vec
vec *vec_new(size_t size) {
  vec *v = (vec *) malloc(sizeof(vec));
  v->cap = 32;
  v->len = 0;
  v->data = calloc(v->cap + 1, size);
  return v;
}

//Delete an existing vec
void vec_del(vec *v, bool is_heap) {
  if (is_heap) {
    for (int i = 0; i < v->len; ++i) {
      //Remove the element if it's on the heap
      free(*(v->data+i));
    }
  } else {
    free(v->data);
  }
  free(v);
}

//Grow the vec by the capacity specified. Returns -1 if it failed
int vec_grow(vec *v, size_t capacity) {
  v->cap += capacity;
  void **p;
  if ((p = realloc(v->data, v->cap+1)) == NULL) {
    v->cap -= capacity;
    return -1;
  }
  //Make sure to set the last value to 0
  v->data[v->cap] = NULL;
  //Assign the new array
  v->data = p;
  return 0;
}

//Shrink the vec by the capacity specified. Returns -1 if it failed
int vec_shrink(vec *v, size_t capacity) {
  //Set capacity to no smaller than the length
  v->cap = (v->cap - capacity < v->len) ? v->len : v->cap - capacity;

  void **p;
  if ((p = realloc(v->data, v->cap+1)) == NULL) {
    v->cap += capacity;
    return -1;
  }

  //Make sure to set the last value to 0
  v->data[v->cap] = NULL;
  //Assign the new array
  v->data = p;
  return 0;
}

//Set the capacity. Will not be smaller than the current length
int vec_set_cap(vec *v, size_t capacity) {
  size_t prev_cap = v->cap;
  //Set capacity to no smaller than the length
  v->cap = (capacity < v->len) ? v->len : capacity;

  void **p;
  if ((p = realloc(v->data, v->cap+1)) == NULL) {
    v->cap = prev_cap;
    return -1;
  }
//  memset()

  //Make sure to set the last value to 0
  v->data[v->cap] = NULL;
  //Assign the new array
  v->data = p;
  return 0;
}

//Get a node at a specific index position
void *vec_get(const vec *v, const unsigned int index) {
  if (index >= v->len) {
    return NULL;
  }

  return v->data[index];
}

//Find the first element in the vec based on comparison function for more advanced checks
//Returns -1 if nothing was found
int vec_find_with(const vec *v, const void *e, int (*cmp)(const void *a, const void *b)) {
  for (int i = 0; i < v->len; ++i) {
    if (cmp(e, v->data[i]) == 0)
      return i;
  }
  return -1;
}

//Insert element after index into vector
//Returns -1 if failed
int vec_insert(vec *v, const unsigned int index, void *e) {
  if (v->len == v->cap) {
    if (vec_grow(v, v->cap*2) == -1) {
      return -1;
    }
  }
  //Increase vector length and add element
  v->data[v->len++] = e;
  return 0;
}

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

//Print the contents of a vec for debugging
void vec_print(const vec *v, const char *format) {
  for (int i = 0; i < v->len; ++i) {
    printf(format, v->data[i]);
  }
}

//Print the contents of a vec between nodes for debugging
void vec_print_between(const vec *v, const vec_node *i, const vec_node *j, const char *format) {
  //Does not flip nodes if they are backwards unlike other function
  for (int idx = i; idx < j; ++idx)
    printf(format, v->data[i]);
}
