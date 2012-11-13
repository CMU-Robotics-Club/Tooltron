#ifndef TOOL_H
#define TOOL_H

enum toolstate_t {
  TS_INIT,
  TS_OFF,
  TS_REQ_DIS,
  TS_ON
};

struct tool_t {
  int address;
  const char *name;
  enum toolstate_t state;
  unsigned int user;
};

int tool_init_mb(const char *device);
void tool_close_mb();
void tool_request_disable(struct tool_t *tool);
void tool_poll(struct tool_t *tool);

#endif
