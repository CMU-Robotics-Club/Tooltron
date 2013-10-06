#include "cache.h"
#include "log.h"
#include <stdlib.h>
#include <time.h>

struct entry_t {
  unsigned int key;
  unsigned int value;
  time_t retrieved;
  struct entry_t *next;
};

static struct entry_t *cache[CACHE_SIZE];
static int n_entries;

static void cache_remove_lru() {
  struct entry_t *entry, **ptr;
  struct entry_t *lru, **lru_ptr;
  int i;

  lru = NULL;
  for (i = 0; i < CACHE_SIZE; i++) {
    ptr = &cache[i];
    for (entry = cache[i]; entry; entry = entry->next) {
      if (lru == NULL || entry->retrieved < lru->retrieved) {
        lru = entry;
        lru_ptr = ptr;
      }
      ptr = &entry->next;
    }
  }

  if (lru != NULL) {
    *lru_ptr = lru->next;
    free(lru);
    n_entries--;
  }
}

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

  n_entries = 0;
}

struct entry_t *cache_find(unsigned int key) {
  struct entry_t *entry;

  for (entry = cache[key % CACHE_SIZE]; entry; entry = entry->next) {
    if (entry->key == key) {
      entry->retrieved = time(NULL);
      return entry;
    }
  }

  return NULL;
}

void cache_add(unsigned int key, unsigned int value) {
  struct entry_t *entry;
  int i;

  entry = malloc(sizeof(struct entry_t));
  if (!entry) {
    log_print("ERROR: Out of memory");
    return;
  }

  if (n_entries == CACHE_MAX_ENTRIES) {
    cache_remove_lru();
  }
  n_entries++;

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
