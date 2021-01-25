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

  //Set head to face tail and vice versa
  l->head->next = l->head->prev = l->tail;
  l->tail->next = l->tail->prev = l->head;
  return l;
}

//Delete an existing list
void list_del(list *l, bool is_heap) {
  //Save the start pointer
  list_node *start = l->head;
  list_node *n = l->head;
  list_node *temp_n = n;
  do {
    //Store pointer to next element
    temp_n = n->next;
    //Remove the element if it's on the heap
    if (is_heap && (n != l->head || n != l->tail))
      free(n->e);

    //Delete n
    free(n);
    //Set it to the next pointer
    n = temp_n;
  } while (n != start);
  free(l);
}

list_node *list_get_at(const list *l, const unsigned int index) {
  if (index >= l->len) {
    return NULL;
  }
  list_node *n;

  bool isForward = true;
  if (l->len / 2 - index < 0)
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

//Find position of node in the list
int list_get_at_pos(const list *l, const list_node* item) {
  int pos = 0;
  for (list_node *n = l->head; n != item; n = n->next)
    ++pos;
  return pos;
}

//Find the first element in the list where simple comparison works
//Returns NULL if nothing was found
list_node *list_find(const list *l, const void *e) {
  list_node *n;
  for (n = l->head; n->e != e || n != l->head; n = n->next);
  if (n == l->head)
    return NULL;
  return n;
}

//Find the first element in the list based on comparison function for more advanced checks
//Returns NULL if nothing was found
list_node *list_find_by(const list *l, const void *e, bool (*cmp)(const void *a, const void *b)) {
  list_node *n;
  for (n = l->head; n != l->head; n = n->next) {
    if (cmp(e, n->e))
      return n;
  }
  return NULL;
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

  //Bump up len
  ++l->len;
}

//Pop item
//Prone to memory leaking if try to free item twice
void *list_pop(list *l, list_node *item) {
  void *e = item->e;
  item->prev->next = item->next;
  item->next->prev = item->prev;
  free(item);
  --l->len;
  return e;
}

//Reverses the list in place
void list_rev(list *l) {
  list_node *n = l->head;
  list_node *temp;

  do {
    //Move forward in the list
    temp = n->next;
    //Flip the pointers
    n->next = n->prev;
    n->prev = temp;
    //Continue to move forward
    n = temp;
  } while (n != l->head);
  //Since the head and tail got reversed, change their order in the list
  temp = l->tail;
  l->head = temp;
  l->tail = n;
}

//Print the contents of a list for debugging
void list_print(const list *l, const char *format) {
  for (list_node *n = l->head->next; n->next != l->head; n = n->next) {
    printf(format, n->e);
  }
}

//Print the contents of a list between nodes for debugging
void list_print_between(const list *l, const list_node *i, const list_node *j, const char *format) {
  //Does not flip nodes if they are backwards unlike other function
  list_node *n = i;
  while(n != j) {
    //Print the format per node
    printf(format, n->e);
    n = n->next;
  }
}

//Print the contents of a list between indices for debugging
void list_print_between_indices(const list *l, unsigned int i, unsigned int j, const char *format) {
  //Flips signs if they are backwards
  if (i > j) {
    list_print_between_indices(l, j, i, format);
  }

  //Get the location of the node
  list_node *n = list_get_at(l, i);
  while(i != j) {
    //Print the format per node
    printf(format, n->e);
    n = n->next;
    ++i;
  }
}
