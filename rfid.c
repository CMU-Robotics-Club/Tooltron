#include <util/delay.h>
#include "rfid.h"
#include "serial.h"

static char read_cmd[] = {'!', 'R', 'W', 1, 32};

static char serno[RFID_SERNO_SIZE];

static void zero_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    serno[i] = 0;
  }
}

static void read_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    serno[i] = serial_read_blocking();
  }
}

static char check_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    if (serno[i] != serial_read_blocking()) {
      return 0;
    }
  }
  return 1;
}

void rfid_init() {
  serial_init();
}

void rfid_read_safe() {
  char n_err = 0, n_ok = 0;
  while (1) {
    serial_write(read_cmd, sizeof(read_cmd));
    if (serial_read_blocking() == RFID_OK) {
      if (n_ok == 0) {
        read_serno();
      } else if (!check_serno()) {
        zero_serno();
        return;
      }
      n_ok++;
      if (n_ok >= RFID_MIN_OK) {
        return;
      }
    } else {
      n_err++;
      if (n_err >= RFID_MAX_ERRS) {
        zero_serno();
        return;
      }
    }
    _delay_ms(40);
  }
}

void rfid_read() {
  while (1) {
    serial_write(read_cmd, sizeof(read_cmd));
    if (serial_read_blocking() == RFID_OK) {
      read_serno();
      return;
    }
    _delay_ms(40);
  }
}

void rfid_get_serno(char *buf) {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    buf[i] = serno[i];
  }
}
