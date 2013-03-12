#ifndef EVENT_H
#define EVENT_H

#include <time.h>

struct event_t {
  unsigned int user;
  int tool_id;
  time_t tstart;
  time_t tend;
  struct event_t *next;
};

struct event_t *event_alloc();
void event_free(struct event_t *event);
void event_q_push(struct event_t *event);
void event_q_process();

#endif
