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
void map_insert(hashmap *m, void *k, size_t key_size, void *v, size_t value_size);

// Get key from the map. If value_size is -1 then the value was not found
void map_get(hashmap *m, void *k, size_t key_size, void **v, size_t *value_size);

// Remove key from the map
void map_remove_with(hashmap *m, void *k, size_t key_size, void (*del) (void *e));

// Remove key from the map
static inline void map_remove(hashmap *m, void *k, size_t key_size) {
  map_remove_with(m, k, key_size, map_simple_entry_remove);
}

// Get pairs in map
list *map_pairs(const hashmap *m);

#endif
