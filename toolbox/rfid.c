#include <string.h>
#include <stdint.h>
#include <util/delay.h>
#include "rfid.h"
#include "serial.h"

static uint8_t read_cmd[] = {'!', 'R', 'W', 1, 32};

static int serno_idx;
static uint8_t serno[RFID_SERNO_SIZE];

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
    _delay_ms(10);
  }
}

void rfid_start_read() {
  serno_idx = -1;
  zero_serno(); // TODO temporary
  serial_flush();
  serial_write(read_cmd, sizeof(read_cmd));
}

char rfid_poll() {
  int c;

  while ((c = serial_read()) >= 0) {

    if (serno_idx < 0) {
      if (c != RFID_OK) {
        zero_serno();
        return 1;
      }
    } else {
      serno[serno_idx] = c;
    }

    serno_idx++;
    if (serno_idx >= RFID_SERNO_SIZE) {
      return 1;
    }

  }

  return 0;
}

void rfid_get_serno(uint8_t *buf) {
  memcpy(buf, serno, sizeof(serno));
}

char rfid_check_serno(uint8_t *buf) {
  return memcmp(buf, serno, sizeof(serno)) == 0;
}

char rfid_nonzero() {
  int i;
  for (i = 0; i < sizeof(serno); i++) {
    if (serno[i]) {
      return 1;
    }
  }
  return 0;
}
