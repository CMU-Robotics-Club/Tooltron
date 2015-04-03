#ifndef QUERY_H
#define QUERY_H

#include "event.h"
#include "tool.h"

int query_init();
void query_cleanup();

/*
 * Request all tools with a toolbox ID from the server
 * and place them in 'tools'.  Returns -1 on error else the
 * number of tools in 'tools'.
 */
int query_tools(struct tool_t*** tools);

int query_user_permission(int tool_id, unsigned int user_id);
int query_add_event(struct event_t *event);

#endif
