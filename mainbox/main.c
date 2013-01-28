#include "tool.h"
#include "query.h"
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <stdio.h>

#define SLEEP_MS 250

static struct tool_t tools[] = {
  TOOL_DECL("testa", 4),
  TOOL_DECL("testb", 5)
};

#define N_TOOLS (sizeof(tools)/sizeof(struct tool_t))

volatile int run = 1;

void sigint(int sig) {
  run = 0;
}

void print_usage(const char *name) {
  printf("Usage: %s [-h] [-d serial_device] [-s db_server[:port]]\n", name);
  printf("       -h prints this message\n");
  printf("       -d specifies the serial port for Modbus\n");
  printf("          defaults to /dev/ttyUSB0\n");
  printf("       -s specifies the server where the CRM is running\n");
  printf("          defaults to minecraft.roboclub.org:8000\n");
}

int main(int argc, char **argv) {
  int i, opt;
  struct sigaction sigact;
  const char *device = "/dev/ttyUSB0";
  const char *server = "minecraft.roboclub.org:8000";

  while ((opt = getopt(argc, argv, "hd:s:")) != -1) {
    switch (opt) {
      case 'h':
        print_usage(argv[0]);
        return 0;
      case 'd':
        device = optarg;
        break;
      case 's':
        server = optarg;
        break;
      default:
        print_usage(argv[0]);
        return 1;
    }
  }

  printf("Serial device: %s\n", device);
  printf("CRM server: http://%s/\n", server);

  bzero(&sigact, sizeof(sigact));
  sigact.sa_handler = sigint;
  sigact.sa_flags = SA_RESTART;
  sigemptyset(&sigact.sa_mask);
  sigaction(SIGINT, &sigact, NULL);

  if (query_init(server)) {
    return 1;
  }

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
