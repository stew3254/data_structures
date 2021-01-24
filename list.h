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
  unsigned int size;
} list;

list *list_new();
void list_del(list *l);
list_node *list_new_node(void *e);
void list_insert(list *l, list_node *item, void *e);
void *list_pop(list *l, list_node *item);

list_node *list_get_at(const list *l, unsigned int index);
void list_print(const list *l, const char *format);
void list_print_between(const list *l, unsigned int i, unsigned int j, const char *format);

static void list_push_front(list *l, void *e) { list_insert(l, l->head, e); }
static void list_push_back(list *l, void *e) { list_insert(l, l->tail, e); }

static void list_pop_front(list *l) { list_pop(l, l->head->next); }
static void list_pop_back(list *l) { list_pop(l, l->tail->prev); }

#endif
