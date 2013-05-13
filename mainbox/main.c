#include "tool.h"
#include "query.h"
#include "cache.h"
#include "log.h"
#include "util.h"
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#define SLEEP_MS 25

static struct tool_t tools[] = {
  TOOL_DECL("Mill", 1),
  TOOL_DECL("Lathe", 2),
  TOOL_DECL("Drill Press", 3),
  TOOL_DECL("Drill Press", 4),
  TOOL_DECL("CNC", 5),
  TOOL_DECL("Bandsaw", 6),
  TOOL_DECL("Circular Saw", 7),
  TOOL_DECL("Wood Band Saw", 8),
  TOOL_DECL("Chop/Miter Saw", 9),
  TOOL_DECL("Belt Sander", 10)
};

#define N_TOOLS (sizeof(tools)/sizeof(struct tool_t))

volatile int run = 1;
volatile int refresh_cache = 0;
volatile int clear_cache = 0;

void sigint(int sig) {
  run = 0;
}

void sigusr1(int sig) {
  refresh_cache = 1;
}

void sigusr2(int sig) {
  clear_cache = 1;
}

void send_signal(int sig) {
  pid_t pid;

  pid = read_pid_file();

  if (pid > 0)
    kill(pid, sig);
}

int tooltron_main(const char *device, const char *server) {
  struct sigaction sigact;
  int i;

  log_print("Serial device: %s", device);
  log_print("CRM server: http://%s/", server);

  bzero(&sigact, sizeof(sigact));
  sigact.sa_flags = SA_RESTART;
  sigemptyset(&sigact.sa_mask);

  sigact.sa_handler = sigint;
  sigaction(SIGINT, &sigact, NULL);
  sigaction(SIGTERM, &sigact, NULL);

  sigact.sa_handler = sigusr1;
  sigaction(SIGUSR1, &sigact, NULL);

  sigact.sa_handler = sigusr2;
  sigaction(SIGUSR2, &sigact, NULL);

  if (query_init(server)) {
    return 1;
  }

  if (tool_init_mb(device)) {
    return 1;
  }

  log_print("Modbus initialized; polling tools...");

  i = 0;
  while (run) {
    tool_poll(&tools[i]);
    event_q_process();
    if (refresh_cache) {
      log_print("Recieved SIGUSR1, refreshing cache content");
      query_refresh_cache();
      refresh_cache = 0;
    }
    if (clear_cache) {
      log_print("Recieved SIGUSR2, clearing cache");
      cache_clear();
      clear_cache = 0;
    }
    usleep(SLEEP_MS * (useconds_t)1000);
    i = (i+1) % N_TOOLS;
  }

  log_print("Disabling tools");
  for (i = 0; i < N_TOOLS; i++) {
    tool_request_disable(&tools[i]);
  }

  log_print("Closing modbus connection");
  tool_close_mb();

  log_print("Exiting");
  query_cleanup();
  cache_clear();
  return 0;
}

char *usage =
"Usage: %s [-h] [-d device] [-s server[:port]] <cmd>\n"
"       -h prints this message\n"
"       -d specifies the serial port for Modbus\n"
"          defaults to /dev/ttyUSB0\n"
"       -s specifies the server where the CRM is running\n"
"          defaults to roboticsclub.org\n"
"       <cmd> can be any of the following:\n"
"          run     runs tooltron if it is not already running\n"
"          refresh signals an already running tooltron to refresh its cache\n"
"          clear   signals an already running tooltron to clear its cache\n";

int main(int argc, char **argv) {
  int opt;
  const char *device = "/dev/ttyUSB0";
  const char *server = "roboticsclub.org";

  while ((opt = getopt(argc, argv, "hd:s:")) != -1) {
    switch (opt) {
      case 'h':
        /* Print usage, not an error */
        printf(usage, argv[0]);
        return 0;
      case 'd':
        device = optarg;
        break;
      case 's':
        server = optarg;
        break;
      default:
        /* Unknown option, error */
        printf(usage, argv[0]);
        return 1;
    }
  }

  if (optind >= argc) {
    /* Not enough arguments, error */
    printf(usage, argv[0]);
    return 1;
  }

  if (strcmp(argv[optind], "refresh") == 0) {
    send_signal(SIGUSR1);
    return 0;
  } else if (strcmp(argv[optind], "clear") == 0) {
    send_signal(SIGUSR2);
    return 0;
  } else if (strcmp(argv[optind], "run") != 0) {
    /* <cmd> is not "refresh", "clear", or "run", error */
    printf(usage, argv[0]);
    return 1;
  }

  if (create_pid_file())
    /* pid file already exists, error */
    return 1;

  return tooltron_main(device, server);
}
