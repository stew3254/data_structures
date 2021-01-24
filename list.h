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
  int size;
} list;

list *list_new();
list_node *list_new_node(void *e);
void list_print(const list *l, const char *format);
void list_insert(list *l, list_node *item, void *e);
void *list_pop(list_node *item);

static void list_push_front(list *l, void *e) { list_insert(l, l->head, e); }
static void list_push_back(list *l, void *e) { list_insert(l, l->tail, e); }

static void list_pop_front(list *l, void *e) { list_pop(l->head); }
static void list_pop_back(list *l, void *e) { list_pop(l->tail); }

#endif
