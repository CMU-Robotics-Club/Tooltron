#ifndef QUERY_H
#define QUERY_H

#include "event.h"

int query_init();
void query_cleanup();
int query_user_permission(int tool_id, unsigned int user_id);
void query_refresh_cache();
int query_add_event(struct event_t *event);

#endif
