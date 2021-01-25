#ifndef list_h
#define list_h

typedef struct ListNode {
  void *e;
  struct ListNode *next;
  struct ListNode *prev;
} list_node;

typedef struct List {
  list_node *head;
  list_node *tail;
  unsigned int len;
} list;

/* Create a list */
list *list_new();

/* Destroy a list and all elements inside */
//Not exactly meant to be called directly, main logic behind destroying the list
void list_del(list *l, bool is_heap);
//Will not remove items that are stack allocated
static void list_del_stack(list *l) { list_del(l, false); }
//Will free elements before removing the nodes and list
static void list_del_heap(list *l) { list_del(l, true); }

//Create a new node to add to the list. Not meant to call this directly
static list_node *list_new_node(void *e) {
  list_node *node_ptr = (list_node*) malloc(sizeof(list_node));
  node_ptr->prev = node_ptr->next = node_ptr;
  node_ptr->e = e;
  return node_ptr;
}

/* Finding nodes / positions */
//Get a node at a specific index position. Best used for inserting and popping,
//but is used in insert_at and pop_at
list_node *list_get_at(const list *l, unsigned int index);
//Find position of node in the list
int list_find_at(const list *l, const list_node* item);

/* Insert into list after specific node pointer */
//Most efficient way to insert new items
void list_insert(list *l, list_node *item, void *e);
//Insert into list at certain position, not very efficient
static void list_insert_at(list *l, unsigned int i, void *e) { list_insert(l, list_get_at(l, i), e); }
//Add something to the beginning of the list
static void list_push_front(list *l, void *e) { list_insert(l, l->head, e); }
//Add something to the end of the list
static void list_push_back(list *l, void *e) { list_insert(l, l->tail, e); }

/* Pop from the list and retrieve the element inside */
//Most efficient way to pop
void *list_pop(list *l, list_node *item);
//Pop from list at certain position, not very efficient
static void *list_pop_at(list *l, unsigned int i) { return list_pop(l, list_get_at(l, i)); }
//Remove something to the beginning of the list
static void list_pop_front(list *l) { list_pop(l, l->head->next); }
//Remove something to the end of the list
static void list_pop_back(list *l) { list_pop(l, l->tail->prev); }

/* Utility functions */
//Reverses the list in place
void list_rev(list *l);
/* Print functions. Prints format per node */
//Prints the contents of the whole list
void list_print(const list *l, const char *format);
//Prints the contents of the whole list between 2 nodes
void list_print_between(const list *l, const list_node *i, const list_node *j, const char *format);
//Prints the contents of the whole list between 2 indices
void list_print_between_indices(const list *l, unsigned int i, unsigned int j, const char *format);


#endif
