#include "util.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char *read_file(const char *filename) {
  int fd, len, size, nread;
  char *str;
  
  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    log_perror("open");
    return NULL;
  }

  len = 0;
  size = 64;
  str = malloc(size);
  if (!str) {
    close(fd);
    return NULL;
  }

  while (1) {
    nread = read(fd, str+len, size-len);
    len += nread;
    if (len == size) {
      size *= 2;
      str = realloc(str, size);
      if (!str)
        return NULL;
    }
    if (nread == 0) {
      str[len] = '\0';
      close(fd);
      return str;
    } else if (nread < 0) {
      log_perror("read");
      free(str);
      close(fd);
      return NULL;
    }
  }
}
