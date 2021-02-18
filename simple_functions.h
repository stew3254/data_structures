#ifndef simple_functions_h
#define simple_functions_h

#include <stddef.h>
#include <stdlib.h>

// Simply return the element passed in.
static inline void *return_elem(void *e) { return e; }

// "Delete" an element allocated on the stack (really just do nothing)
static inline void do_not_del(void *e) {}

// "Delete" a primitive type allocated on the heap
static inline void free_del(void *e) { free(e); }

// Simple comparison function
static inline int simple_cmp(const void *a, const void *b) {
  if (a > b)
    return 1;
  else if (a == b)
    return 0;
  else
    return -1;
}

// Generic swap function
static inline void swap_ptr(void **a, void **b) {
  void *temp = *a;
  *a = *b;
  *b = temp;
}

// Max
static inline void *max(void *a, void *b) { return (a > b) ? a : b; }
// Min
static inline void *min(void *a, void *b) { return (a > b) ? b : a; }

#endif
