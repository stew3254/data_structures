#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"

//Initialize a new empty list
list *list_new() {
  list *l = (list *) malloc(sizeof(list));
  //Get 2 different pointers for checking later
  l->head = list_new_node(NULL);
  l->tail = list_new_node(NULL);

  l->head->next = l->head->prev = l->tail;
  l->tail->next = l->tail->prev = l->head;
  return l;
}

//Delete an existing list
void list_del(list *l) {
  list_node *start = l->head;
  list_node *n = l->head;
  list_node *temp_n = n;
  do {
    //Store pointer to next element
    temp_n = n->next;
    //Delete n
    free(n);
    //Set it to the next pointer
    n = temp_n;
  } while (n != start);
  free(l);
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
  for (list_node *n = l->head->next; n->next != l->head; n = n->next) {
    printf(format, n->e);
  }
}

list_node *list_get_at(const list *l, const unsigned int index) {
  if (index >= l->size) {
    return NULL;
  }
  list_node *n;

  bool isForward = true;
  if (l->size/2 - index < 0)
    isForward = false;

  if (isForward) {
    n = l->head->next;
    for (int i = 0; i < index; i++) {
      n = n->next;
    }
  } else {
    n = l->tail->prev;
    for (int i = 0; i < index; i++) {
      n = n->prev;
    }
  }
  return n;
}

//Print the contents of a list for debugging
void list_print_between(const list *l, unsigned int i, unsigned int j, const char *format) {
  //Flips signs if they are backwards
  if (i > j) {
    list_print_between(l, j, i, format);
  }

  list_node *n = list_get_at(l, i);
  while(i != j) {
    printf(format, n->e);
    n = n->next;
    ++i;
  }
}

//Insert element after item
void list_insert(list *l, list_node *item, void *e) {
  list_node *new_node = list_new_node(e);

  //Shift the pointer so it points to the element before the fake tail
  if (item == l->tail)
    item = item->prev;

  //Set back pointer to the item being inserted after
  new_node->prev = item;
  //Set next pointer to the item after the one we're inserting after
  new_node->next = item->next;
  //Set the forward pointer of item and back pointer of the item after the new node to the new node
  item->next = new_node->next->prev = new_node;

  //Bump up size
  ++l->size;
}

//Pop item
//Prone to memory leaking if try to free item twice
void *list_pop(list *l, list_node *item) {
  void *e = item->e;
  item->prev->next = item->next;
  item->next->prev = item->prev;
  free(item);
  --l->size;
  return e;
}

