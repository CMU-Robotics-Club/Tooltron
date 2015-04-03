#ifndef TOOL_H
#define TOOL_H

#include <stdbool.h>

enum toolstate_t {
  TS_INIT,
  TS_OFF,
  TS_REQ_DIS,
  TS_ON
};

struct tool_t {
  char* id;
  char *name; //'type' on the server
  int address; //'toolbox_id' on the server
  int connected;
  enum toolstate_t state;
  unsigned int user;
  struct event_t *event;
  bool powered;
};

int tool_init_mb(const char *device);
void tool_close_mb();
void tool_request_disable(struct tool_t *tool);
void tool_poll(struct tool_t *tool);

/* Should be less than 1000 (see modbus_set_response_timeout()). Should also be
 * high enough to ensure that connected tools will respond. Tools poll their own
 * modbus code about every 50ms. */
#define MB_TIMEOUT_MS 300

#endif
