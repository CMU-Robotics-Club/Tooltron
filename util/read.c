#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
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

int main(int argc, char **argv) {
  int fd;
  char status;

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
  init_tty(fd);

  while (1) {
    tcflush(fd, TCIFLUSH);
    write(fd, read_cmd, sizeof(read_cmd));
    read_all(fd, &status, 1);
    if (status == RFID_OK) {
      read_all(fd, &serno, 4);
      serno = bswap(serno);
      printf("%08x\n", serno);
    } else {
      printf("error %d\n", (int)(unsigned char)status);
    }
    usleep(50000);
  }

  close(fd);
  return 0;
}
