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

/*
 * After event_alloc, you must call either event_free or event_q_push.
 * Periodically call event_q_process to send events in the queue to the server.
 * When an event is successfully sent, the event will be removed from the queue
 * and freed.
 */
struct event_t *event_alloc();
void event_free(struct event_t *event);
void event_q_push(struct event_t *event);
void event_q_process();

#endif
