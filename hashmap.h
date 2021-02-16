#ifndef HASHMAP_H
#define HASHMAP_H

#include "simple_functions.h"
#include "avl.h"
#include "list.h"

typedef struct HashMap {
  avl_tree **buckets;
  unsigned int bucket_size;
  unsigned long (*hash) (const void* k, size_t n);
  unsigned int len;
} hashmap;

typedef struct Entry {
  void *key;
  void *value;
  size_t key_size;
  size_t value_size;
} hashmap_entry;


/* Utility functions */
// Default hashing function
unsigned long hashpjw(const void *k, size_t n);
// Comparison function for hashmap entries
// Needed in key searching / sorting in the tree
int map_simple_entry_cmp(const void *a, const void *b);
// Simple entry removal
static inline void map_simple_entry_remove(void *e) {
  // Free the key and value
  free(((hashmap_entry *) e)->key);
  free(((hashmap_entry *) e)->value);
  free(e);
}
// Preserve value on removal. Used when you get something from the map first
// And then want to use that same pointer later without copying
static inline void map_value_preserve_entry_remove(void *e) {
  // Free the key and value
  free(((hashmap_entry *) e)->key);
  free(e);
}
// Used when both things came from the stack
static inline void map_stack_entry_remove(void *e) {
  // Simply free the entry
  free(e);
}

// Assigning a different hashing function for the hashmap
hashmap *map_with_func(size_t size, unsigned long (*hash) (const void* k, size_t n));

// Hashmap with the standard hash function, but different size
static inline hashmap *map_with_size(size_t size) {
  return map_with_func(size, hashpjw);
}

// Create a new hashmap
static inline hashmap *map_new() {
  return map_with_size(211);
}

// Delete a hashmap
static inline void map_del(hashmap *m, void (*del) (void *e)) {
  // Free all of the trees
  for (size_t i = 0; i < m->bucket_size; ++i)
    avl_tree_del(m->buckets[i], del);

  //Free the buckets
  free(m->buckets);
  // Free the map itself
  free(m);
}

// Insert into the map
// The key must be a pointer to the thing you actually want to use
void map_insert_with(
    hashmap *m,
    void **k,
    size_t key_size,
    void *v,
    size_t value_size,
    void (*del) (void *e)
);

// Get key from the map. If value_size is -1 then the value was not found
// The key must be a pointer to the thing you actually want to use
void map_get(hashmap *m, void **k, size_t key_size, void **v, size_t *value_size);

// Remove key from the map
// The key must be a pointer to the thing you actually want to use
void map_remove_with(hashmap *m, void **k, size_t key_size, void (*del) (void *e));

// Remove key from the map
// The key must be a pointer to the thing you actually want to use
static inline void map_remove(hashmap *m, void *k, size_t key_size) {
  map_remove_with(m, k, key_size, map_simple_entry_remove);
}

// Get pairs in map
static inline list *map_pairs(const hashmap *m) {
  list *l = avl_tree_to_list(m->buckets[0]);
  for (unsigned int i = 1; i < m->bucket_size; ++i)
    l = list_concat_consume(l, avl_tree_to_list(m->buckets[i]), return_elem);
  return l;
}
// Get keys in map
list *map_keys(const hashmap *m);
// Get values in map
list *map_values(const hashmap *m);

/* Printing utilities */
// Print a hashmap
void map_print(const hashmap *m, char *key_format, char *value_format);
// Print a hashmap but with a newline at the end
static inline void map_println(const hashmap *m, char *key_format, char *value_format) {
  map_print(m, key_format, value_format);
  printf("\n");
}
// Print keys of a hashmap
void map_keys_print(const hashmap *m, char *format);
// Print a hashmap but with a newline at the end
static inline void map_keys_println(const hashmap *m, char *format) {
  map_keys_print(m, format);
  printf("\n");
}
// Print values of a hashmap
void map_values_print(const hashmap *m, char *format);
// Print a hashmap but with a newline at the end
static inline void map_values_println(const hashmap *m, char *format) {
  map_values_print(m, format);
  printf("\n");
}

#endif
