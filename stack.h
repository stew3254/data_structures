#ifndef STACK_H
#define STACK_H

#include "stdlib.h"
#include "stdbool.h"

typedef struct Stack {
  char **buffer;
  int len;
  int cap;
} stack;

static inline int scap(stack *s) {return s->cap;}
static inline int slen(stack *s) {return s->len;}
static inline bool sisempty(stack *s) {return (bool) (!s->len);}
static inline bool sisfull(stack *s) {return (s->len == s->cap);}
static inline char *sget(stack *s) {return s->buffer[s->len-1];}

static inline void ssetcap(stack *s, int cap) {
  s->buffer = (char**) realloc(s->buffer, cap*sizeof(char*));
  s->cap = cap;
}

static inline void spush(stack *s, char *e) {
  if (s->len == s->cap)
    ssetcap(s, 2*s->cap);
  s->buffer[s->len++] = e;
}

static inline char *spop(stack *s) {
  return s->buffer[--s->len];
}

static inline stack *news(int cap) {
  stack *s = (stack*) malloc(sizeof(stack));
  s->buffer = (char**) malloc(cap*sizeof(char*));
  s->cap = cap;
  s->len = 0;
  return s;
}

static inline void dels(stack *s) {
  free(s->buffer);
  free(s);
}


#endif
