#ifndef STACKED_TABLES_H
#define STACKED_TABLES_H

#include <stdlib.h>
#include "stack.h"
#include "list.h"
#include "hashmap.h"

//Define the TableStack type
typedef stack table_stack;

typedef struct Symbol {
    int type;
    union {
        int ival;
        float rval;
        int opval;
        char *sval;
    } attribute;
} symbol;

symbol *symbol_new(int type) {
    symbol *new_s = (symbol *) malloc(sizeof(symbol));
    new_s->type = type;
    return new_s;
}

// TODO fix this when you can tell it's a string for proper string copy
void *symbol_copy(const void *e) {
    symbol *s = (symbol*) e;
    // Create the new symbol
    symbol *new_s = symbol_new(s->type);
    new_s->attribute.sval = s->attribute.sval;
    return new_s;
}

// TODO fix this for when you can tell it's a string for proper deletion
void symbol_del(void *e) {
    // free the struct
    free(e);
}

// Create a new table_stack
static inline table_stack *table_stack_new() {
    table_stack *t = (table_stack*) stack_new(symbol_copy, symbol_del);
    return t;
}
// Delete a table_stack
static inline void table_stack_del(table_stack *s) {
    list_del(s->data);
    free(s);
}

// Get the length of the table_stack
static inline unsigned int table_stack_len(table_stack *s) {
    return s->data->len;
}

// Get the top element off of the table_stack
static inline hashmap *table_stack_get(table_stack *s) {
    return s->data->head->next->e;
}

// Push an element onto the table_stack
static inline void table_stack_push(table_stack *s, hashmap *scope) {
    list_push_front(s->data, scope);
}

// Pop an element off of the table_stack
static inline hashmap *table_stack_pop(table_stack *s) {
    return list_pop_front(s->data);
}

// Copy the table_stack
static inline table_stack *table_stack_copy(table_stack *s) {
    return stack_copy(s);
}

// Print the table_stack
static inline void table_stack_print(table_stack *s, char *format) {
    for (list_node *n = s->data->head->next; n->next != s->data->head; n = n->next) {
        hashmap *scope = n->e;
        map_print(scope, "%s", "%p");
        if (n->next != s->data->head)
            printf(",\n");
    }
}

// Print the table_stack with a newline
static inline void table_stack_println(table_stack *s, char *format) {
    list_println(s->data, format);
}

#endif
