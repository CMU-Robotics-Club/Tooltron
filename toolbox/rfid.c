#include <string.h>
#include <stdint.h>
#include <util/delay.h>
#include "rfid.h"
#include "serial.h"

#define RFID_OK 1
static uint8_t read_cmd[] = {'!', 'R', 'W', 1, 32};

static int serno_idx;
static char n_failures;
static uint8_t serno[RFID_SERNO_SIZE];

static void zero_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    serno[i] = 0;
  }
}

void rfid_init() {
  serial_init();
}

static void restart_read() {
  serno_idx = -1;
  serial_write(read_cmd, sizeof(read_cmd));
}

void rfid_start_read() {
  n_failures = 0;
  restart_read();
}

char rfid_poll() {
  int c;

  while ((c = serial_read()) >= 0) {

    if (serno_idx < 0) {
      if (c != RFID_OK) {
        n_failures++;
        if (n_failures >= RFID_N_FAILURES) {
          zero_serno();
          return 1;
        } else {
          restart_read();
          return 0;
        }
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
