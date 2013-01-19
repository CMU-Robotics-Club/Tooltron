#ifndef QUERY_H
#define QUERY_H

int query_init();
void query_cleanup();
int query_user_permission(int tool_id, int user_id);

#endif
