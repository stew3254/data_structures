//
// Created by stew3254 on 1/25/21.
//

#ifndef simple_functions_h
#define simple_functions_h

#include <stddef.h>

//Simply return the element passed in.
static void *simple_copy(const void *e) { return e; }

//Simple comparison function
static int simple_cmp(const void *a, const void *b) {
  if (a > b)
    return 1;
  else if (a == b)
    return 0;
  else
    return -1;
}

//Cursed swap function. Only works on things of exactly sizeof(long) == ptr
static void swap_ptr(void **a, void **b) {
  *a = (void *) ((ptrdiff_t) (*a) ^ (ptrdiff_t) (*b));
  *b = (void *) ((ptrdiff_t) (*a) ^ (ptrdiff_t) (*b));
  *a = (void *) ((ptrdiff_t) (*a) ^ (ptrdiff_t) (*b));
}

#endif
