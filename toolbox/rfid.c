#include <string.h>
#include <stdint.h>
#include <util/delay.h>
#include "rfid.h"
#include "serial.h"

#define MF_SELECT_CMD 0x83

static uint8_t select_cmd[] = {0xFF, 0, 1, MF_SELECT_CMD, MF_SELECT_CMD + 1};

#define FRAME_SIZE(data_size) ((data_size)+4)
#define RFID_DATA_SIZE (RFID_SERNO_SIZE+2)
#define MAX_FRAME_SIZE FRAME_SIZE(RFID_DATA_SIZE)

#define RESP_START    0
#define RESP_RESERVED 1
#define RESP_LENGTH   2
#define RESP_CMD      3
#define RESP_DATA     4
static uint8_t response[MAX_FRAME_SIZE];
static uint8_t resp_idx;

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

void rfid_start_read() {
  resp_idx = 0;
  serial_flush();
  serial_write(select_cmd, sizeof(select_cmd));
}

char parse_response() {
  uint8_t sum;
  int resp_csum, i;

  if (response[RESP_CMD] != MF_SELECT_CMD) {
    // invalid response, ignore
    zero_serno();
    return 0;
  }

  if (response[RESP_LENGTH] != RFID_DATA_SIZE) {
    // wrong response length, assume there's no card
    zero_serno();
    return 1;
  }

  resp_csum = response[RESP_LENGTH] + RESP_CMD;
  sum = 0;
  for (i = RESP_LENGTH; i < resp_csum; i++) {
    sum += response[i];
  }
  if (response[resp_csum] != sum) {
    zero_serno();
    return 0;
  }

  memcpy(serno, &response[RESP_DATA+1], RFID_SERNO_SIZE);
  return 1;
}

char rfid_poll() {
  int c;

  while ((c = serial_read()) >= 0) {

    if (resp_idx < sizeof(response)) {
      response[resp_idx] = c;
    }
    resp_idx++;

    if (resp_idx == 1) {

      // restart if the frame start is invalid
      if (response[RESP_START] != 0xFF) {
        resp_idx = 0;
      }

    } else if (resp_idx > RESP_LENGTH) {

      // check if we're done with current packet
      if (resp_idx >= FRAME_SIZE(response[RESP_LENGTH])) {
        rfid_start_read();
        if (parse_response()) {
          return 1;
        }
      }

    }
  }

  return 0;
}

void rfid_get_serno(uint8_t *buf) {
  memcpy(buf, serno, sizeof(serno));
}
