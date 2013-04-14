#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define RFID_OK 1
static char read_cmd[] = {'!', 'R', 'W', 1, 32};
static int serno;

void init_tty(int fd) {
  struct termios ios;
  bzero(&ios, sizeof(ios));
  ios.c_cflag = B9600 | CS8 | CREAD;
  tcsetattr(fd, TCSAFLUSH, &ios);
}

int bswap(int x) {
  return ((x&0xff) << 24) | ((x&0xff00) << 8)
    | ((x>>8) & 0xff00) | ((x>>24) & 0xff);
}

void read_all(int fd, void *buf, int n) {
  int nread;
  while (n > 0) {
    nread = read(fd, buf, n);
    buf += nread;
    n -= nread;
    usleep(10000);
  }
}

void write_all(int fd, void *buf, int n) {
  int nwritten;
  while (n > 0) {
    nwritten = write(fd, buf, n);
    buf += nwritten;
    n -= nwritten;
    usleep(10000);
  }
}

void fail_error(const char *error, ...) {
  va_list args;
  va_start(args, error);
  printf("Status: 500 Internal Server Error\n");
  printf("Content-type: text/plain\n\n");
  vprintf(error, args);
  va_end(args);
  exit(0);
}

int main(int argc, char **argv) {
  int fd;
  char status;

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
  if (fd < 0)
    fail_error("Could not open /dev/ttyUSB0\n");

  init_tty(fd);
  tcflush(fd, TCIFLUSH);
  write_all(fd, read_cmd, sizeof(read_cmd));

  read_all(fd, &status, 1);
  if (status == RFID_OK) {
    read_all(fd, &serno, 4);
    serno = bswap(serno);
    printf("Content-type: text/plain\n\n");
    printf("%08x\n", serno);
  } else {
    fail_error("Reader gave error %d\n", (int)(unsigned char)status);
  }

  close(fd);
  return 0;
}
