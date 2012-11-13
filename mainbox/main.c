#include "tool.h"
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <stdio.h>

#define SLEEP_MS 250

struct tool_t tools[] = {
  {4, "test", TS_INIT}
};

#define N_TOOLS (sizeof(tools)/sizeof(struct tool_t))

volatile int run = 1;

void sigint(int sig) {
  run = 0;
}

int main(int argc, char **argv) {
  int i;
  struct sigaction sigact;
  const char *device = "/dev/ttyUSB0";

  // TODO getopts to get device name

  bzero(&sigact, sizeof(sigact));
  sigact.sa_handler = sigint;
  sigact.sa_flags = SA_RESTART;
  sigemptyset(&sigact.sa_mask);
  sigaction(SIGINT, &sigact, NULL);

  if (tool_init_mb(device)) {
    return 1;
  }

  printf("Modbus initialized; polling tools...\n");

  i = 0;
  while (run) {
    tool_poll(&tools[i]);
    usleep(SLEEP_MS * (useconds_t)1000);
    i = (i+1) % N_TOOLS;
  }

  printf("\nDisabling tools\n");
  for (i = 0; i < N_TOOLS; i++) {
    tool_request_disable(&tools[i]);
  }

  printf("Closing modbus connection\n");
  tool_close_mb();

  printf("Exiting\n");
  return 0;
}
