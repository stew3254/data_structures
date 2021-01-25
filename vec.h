#ifndef vec_h
#define vec_h

#include <stdbool.h>
#include "simple_functions.h"

typedef struct Vec {
    void *data;
    unsigned int capacity;
    unsigned int len;
} vec;

/* Create a vec of correct data type size */
vec *vec_new(size_t size);

/* Destroy a vec and all elements inside */
//Not exactly meant to be called directly, main logic behind destroying the vec
void vec_del(vec *v, bool is_heap);
//Will not remove items that are stack allocated
static void vec_del_stack(vec *v) { vec_del(v, false); }
//Will free elements before removing the nodes and vec
static void vec_del_heap(vec *v) { vec_del(v, true); }

/* Finding nodes / positions */
//Get a node at a specific index position
void *vec_get(const vec *v, unsigned int index);

/* Simple searching functions */
//Find the first element in the vec based on comparison function for more advanced checks
//Returns NULL if nothing was found
int vec_find_with(const vec *v, const void *e, int (*cmp)(const void *a, const void *b));
//Find the first element in the vec where simple comparison works
//Returns NULL if nothing was found
static int vec_find(const vec *v, const void *e) { return vec_find_with(v, e, simple_cmp); }

/* Insert into vec after specific node pointer */
//Inserts item into vector
void vec_insert(vec *l, unsigned int index, void *e);
//Add something to the end of the vec
static void vec_push_back(vec *v, void *e) { vec_insert(v, v->len, e); }

/* Pop from the vec and retrieve the element inside */
//Pops the element in the vector
void *vec_pop(vec *v, unsigned int index);
//Remove something to the end of the vec
static void vec_pop_back(vec *v) { vec_pop(v, v->len - 1); }

/* Utility functions */
//Reverses the vec in place
void vec_rev(vec *v);

//Does a deep copy of elements into a new vec. Allows you to specify how to deep copy
vec *vec_copy_with(const vec *v, void *(copy)(const void *e));
//Does a shallow copy of elements into a new vec. Works well for simple types
static vec *vec_copy(const vec *v) { return vec_copy_with(v, simple_copy); }

//Concatenates 2 vecs with copy function and returns a new vec
vec *vec_concat_with(vec *v1, vec* v2, void *(copy)(const void *e));
//Concatenates 2 vecs and returns a new vec
static vec *vec_concat(vec *v1, vec* v2) { return vec_concat_with(v1, v2, simple_copy); }

/* Print functions. Prints format per node */
//Prints the contents of the whole vec
void vec_print(const vec *v, const char *format);
//Prints the contents of the whole vec with a newline at the end
static void vec_println(const vec *v, const char *format) {
  vec_print(l, format);
  printf("\n");
}
//Prints the contents of the whole vec between 2 indices
void vec_print_between(const vec *l, unsigned int i, unsigned int j, const char *format);
//Prints the contents of the whole vec between 2 indices with a newline at the end
static void vec_println_between_indices(const vec *l, unsigned int i, unsigned int j, const char *format) {
  vec_print_between(l, i, j, format);
  printf("\n");
}

#endif