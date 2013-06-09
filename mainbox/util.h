#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>

#define PIDFILE "/var/run/tooltron.pid"

char *read_file(const char *filename);
int create_pid_file();
pid_t read_pid_file();
void remove_pid_file();

#endif
