#ifndef CACHE_H
#define CACHE_H

#define CACHE_SIZE 256
#define CACHE_MAX_ENTRIES 160

typedef void (*cache_func)(unsigned int key);

void cache_foreach(cache_func f);
void cache_clear();
int cache_lookup(unsigned int rfid, unsigned int tool_id, unsigned int *value);
void cache_update(unsigned int rfid, unsigned int tool_id, unsigned int value);

#endif
