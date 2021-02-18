#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

typedef struct Stack {
    list *data;
    int len;
    void *(*copy) (const void *e);
    void (*del) (void *e);
} stack;

// Create a enw stack
static inline stack *stack_new(void *(*copy) (const void *e), void (*del) (void *e)) {
  stack *s = malloc(sizeof(stack));
  // We don't actually need to do any comparisons
  s->data = list_new(simple_cmp, s->copy = copy, s->del = del);
  s->len = 0;
  return s;
}

// Delete a stack
static inline void stack_del(stack *s) {
  list_del(s->data);
  free(s);
}

// Get the top element off of the stack
static inline void *stack_get(stack *s) {
  return s->data->head->next->e;
}

// Push an element onto the stack
static inline void stack_push(stack *s, void *e) {
  list_push_front(s->data, e);
}

// Pop an element off of the stack
static inline void *stack_pop(stack *s) {
  return list_pop_front(s->data);
}

// Copy the stack
static inline stack *stack_copy(stack *s) {
  stack *new_s = stack_new(s->copy, s->del);
  new_s->data = list_copy(s->data);
  new_s->len = s->len;
  return s;
}

// Print the stack
static inline stack *stack_print(stack *s, char *format) {
  list_print(s->data, format);
}

// Print the stack with a newline
static inline stack *stack_println(stack *s, char *format) {
  list_println(s->data, format);
}

#endif
