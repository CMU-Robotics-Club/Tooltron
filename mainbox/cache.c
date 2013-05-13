#include "cache.h"
#include "log.h"
#include <stdlib.h>

struct entry_t {
  unsigned int key;
  unsigned int value;
  time_t retrieved;
  struct entry_t *next;
};

struct entry_t *cache[CACHE_SIZE];

void cache_foreach(cache_func f) {
  struct entry_t *entry;
  int i;

  for (i = 0; i < CACHE_SIZE; i++) {
    for (entry = cache[i]; entry; entry = entry->next)
      f(entry->key);
  }
}

void cache_clear() {
  struct entry_t *entry, *next;
  int i;

  for (i = 0; i < CACHE_SIZE; i++) {
    for (entry = cache[i]; entry; entry = next) {
      next = entry->next;
      free(entry);
    }
    cache[i] = NULL;
  }
}

struct entry_t *cache_find(unsigned int key) {
  struct entry_t *entry;

  for (entry = cache[key % CACHE_SIZE]; entry; entry = entry->next) {
    if (entry->key == key)
      return entry;
  }

  return NULL;
}

void cache_add(unsigned int key, unsigned int value) {
  struct entry_t *entry;
  int i;

  entry = malloc(sizeof(struct entry_t));
  if (!entry) {
    log_print("ERROR: Out of memory; clearing cache");
    cache_clear();
    return;
  }

  i = key % CACHE_SIZE;
  entry->key = key;
  entry->value = value;
  entry->next = cache[i];
  cache[i] = entry;
}

int cache_lookup(unsigned int key, unsigned int *value) {
  struct entry_t *entry;
  
  entry = cache_find(key);
  if (entry) {
    *value = entry->value;
    return 1;
  } else
    return 0;
}

void cache_update(unsigned int key, unsigned int value) {
  struct entry_t *entry;

  entry = cache_find(key);
  if (entry)
    entry->value = value;
  else
    cache_add(key, value);
}
