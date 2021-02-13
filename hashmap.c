#include "avl.h"
#include "hashmap.h"
#include "stdio.h"

//Default hashing function
unsigned long hashpjw(const void *k, size_t n) {
  unsigned long high;
  unsigned long  h = 0;

  for (size_t i = 0; i < n; ++i) {
    // Casting to a char pointer so I can only index a single byte at once
    h = (unsigned long) ((h << 4) + ((char *) *((char *) k) + i));
    if ((high = h & 0xF0000000) > 0)
      h ^= high >> 24;
    h &= ~high;
  }
  return h;
}

// Comparison function for hashmap entries
// Needed in key searching / sorting in the tree
int map_simple_entry_cmp(const void *a, const void *b) {
  // Convert to a byte array and compare byte by byte
  char *ak = (char *) ((hashmap_entry *) a)->key;
  char *bk = (char *) ((hashmap_entry *) b)->key;
  size_t a_size = (size_t) ((hashmap_entry *) a)->key_size;
  size_t b_size = (size_t) ((hashmap_entry *) b)->key_size;
  for (size_t i = 0; i < min(a_size, b_size); ++i) {
    if (ak[i] > bk[i])
      return 1;
    else if (ak[i] < bk[i])
      return -1;
  }

  // Make sure one is not longer than the other
  if (a_size > b_size)
    return 1;
  else if (a_size < b_size)
    return -1;

  // They truly are the same, byte by byte and by length
  return 0;
}

hashmap *map_with_func(size_t size, unsigned long (*hash) (const void* k, size_t n)) {
  hashmap *m = (hashmap *) malloc(sizeof(hashmap));
  m->hash = hash;
  m->bucket_size = size;
  m->len = 0;
  m->buckets = malloc(sizeof(avl_tree)*(size+1));
  // Initialize all of the trees
  for (size_t i = 0; i < size; ++i) {
    m->buckets[i] = avl_tree_new();
  }
  // Set last bucket to NULL just in case
  m->buckets[size] = NULL;
  return m;
}

// Insert into the map
void map_insert(hashmap *m, void *k, size_t key_size, void *v, size_t value_size) {
  unsigned int index = hashpjw(k, key_size) % m->bucket_size;
  avl_tree *bucket = m->buckets[index];

  // Initialize the entry
  hashmap_entry *e = (void *) malloc(sizeof(hashmap_entry));
  e->key = k;
  e->key_size = key_size;
  e->value = v;
  e->value_size = value_size;

  // Insert into the bucket
  avl_tree_insert_with(bucket, e, map_simple_entry_cmp);
}

// Get key from the map. If value_size is -1 then the value was not found
void map_get(hashmap *m, void *k, size_t key_size, void **v, size_t *value_size) {
  unsigned int index = hashpjw(k, key_size) % m->bucket_size;
  avl_tree *bucket = m->buckets[index];

  // Make entry to search with
  hashmap_entry entry = {
    .key = k,
    .key_size = key_size,
  };

  // Get the result from our tree
  search_result r = avl_tree_get_with(bucket, &entry, map_simple_entry_cmp);

  // Set the value if we found it
  if (r.found) {
    *v = ((hashmap_entry *) r.e)->value;
    *value_size = ((hashmap_entry *) r.e)->value_size;
  } else {
    // Alert the user we did not find it
    *value_size = -1;
  }
}
