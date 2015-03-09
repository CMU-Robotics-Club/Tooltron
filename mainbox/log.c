#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>

void log_time() {
  char buf[100];
  time_t t;
  time(&t);
  strftime(buf, sizeof(buf), "%b %e %T %Y", localtime(&t));
  printf("[%s] ", buf);
}

void log_print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  log_time();
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
  fflush(stdout);
}

void log_perror(const char *s) {
  log_time();
  printf("%s: %s\n", s, strerror(errno));
}
