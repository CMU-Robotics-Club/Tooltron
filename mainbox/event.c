#include "event.h"
#include "query.h"
#include <stdlib.h>

struct event_t *head, *tail;

struct event_t *event_alloc() {
  return malloc(sizeof(struct event_t));
}

void event_free(struct event_t *event) {
  free(event);
}

void event_q_push(struct event_t *event) {
  if (tail)
    tail->next = event;
  else
    head = event;
  tail = event;
  event->next = NULL;
}

void event_q_process() {
  struct event_t *old;
  if (head && query_add_event(head) == 0) {
    old = head;
    head = head->next;
    event_free(old);
  }
}
