#include "util.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1024

/*
 * read_file
 *
 * Reads the first line of a file. Returns NULL on error, or a pointer to a
 * malloc'd string which should later be freed.
 */
char *read_file(const char *filename) {
  char buf[BUF_SIZE];
  int len;
  FILE *file;

  file = fopen(filename, "r");
  if (file == NULL) {
    log_perror(filename);
    return NULL;
  }

  fgets(buf, BUF_SIZE, file);

  if (ferror(file)) {
    log_perror(filename);
    fclose(file);
    return NULL;
  }

  len = strlen(buf);
  if (len > 0 && buf[len-1] == '\n')
    buf[len-1] = '\0';

  fclose(file);
  return strdup(buf);
}

/*
 * create_pid_file
 *
 * Creates PIDFILE containing the PID of the current process.  Returns 0 if
 * successful, or nonzero if there is an error or it already exists.
 */
int create_pid_file() {
  int fd;
  FILE *file;
  
  fd = open(PIDFILE, O_CREAT | O_EXCL | O_WRONLY, 0644);
  if (fd < 0) {
    if (errno == EEXIST)
      fprintf(stderr, "ERROR: tooltron is already running, or the pidfile "
          PIDFILE " is stale");
    else
      log_perror(PIDFILE);
    return 1;
  }

  file = fdopen(fd, "w");
  if (!file) {
    log_perror("fdopen");
    return 1;
  }

  fprintf(file, "%d", getpid());

  fclose(file);
  return 0;
}

/*
 * remove_pid_file
 *
 * Removes PIDFILE.
 */
void remove_pid_file() {
  if (unlink(PIDFILE))
    log_perror(PIDFILE);
}

/*
 * read_pid_file
 *
 * Returns the integer found in PIDFILE, or 0 if there was an
 * error.
 */
pid_t read_pid_file() {
  FILE *file;
  int pid;

  file = fopen(PIDFILE, "r");
  if (!file) {
    if (errno == ENOENT)
      fprintf(stderr, "ERROR: tooltron does not appear to be running\n");
    else
      perror(PIDFILE);
    return 0;
  }

  if (fscanf(file, "%d", &pid) != 1) {
    perror("fscanf");
    fclose(file);
    return 0;
  }

  fclose(file);
  return pid;
}
