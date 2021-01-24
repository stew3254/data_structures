#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

//Initialize a new empty list
list *list_new() {
  list *l = (list *) malloc(sizeof(list));
  l->head = l->tail = NULL;
  return l;
}

//Create a new node to add to the list
list_node *list_new_node(void *e) {
  list_node *node_ptr = (list_node*) malloc(sizeof(list_node));
  node_ptr->prev = node_ptr->next = node_ptr;
  node_ptr->e = e;
  return node_ptr;
}

//Print the contents of a list for debugging
void list_print(const list *l, const char *format) {
  for (list_node *n = l->head; n->next != l->head; n = n->next) {
    printf(format, n->e);
  }
}

//Insert element after item
void list_insert(list *l, list_node *item, void *e) {
  list_node *ret = list_new_node(e);
  ret->prev = item;
  ret->next = item->next;
  ret->next->prev = ret;
  item->next = ret;
}

//Pop item
//Prone to memory leaking if try to free item twice
void *list_pop(list_node *item) {
  void *e = item->e;
  item->prev->next = item->next;
  item->next->prev = item->prev;
  free(item);
  return e;
}

