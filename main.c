#include <stdint.h>
#include <util/delay.h>
#include "mb.h"
#include "mbport.h"
#include "tooltron_mb.h"
#include "serial.h"

#define RFID_OK 1

static char rfid_read_cmd[] = {'!', 'R', 'W', 1, 32};

#define RFID_SERNO_SIZE 4
static char rfid_serno[RFID_SERNO_SIZE];

static void rfid_zero_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    rfid_serno[i] = 0;
  }
}

static void rfid_read_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    rfid_serno[i] = serial_read_blocking();
  }
}

static char rfid_check_serno() {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    if (rfid_serno[i] != serial_read_blocking()) {
      return 0;
    }
  }
  return 1;
}

/*
 * When reading, it must read the same value RFID_MIN_OK times. If it
 * encounters RFID_MAX_ERRS read errors first, it will output all 0's.
 */
#define RFID_MAX_ERRS 10
#define RFID_MIN_OK 5

static void rfid_read_safe() {
  char n_err = 0, n_ok = 0;
  while (1) {
    serial_write(rfid_read_cmd, sizeof(rfid_read_cmd));
    if (serial_read_blocking() == RFID_OK) {
      if (n_ok == 0) {
        rfid_read_serno();
      } else if (!rfid_check_serno()) {
        rfid_zero_serno();
        return;
      }
      n_ok++;
      if (n_ok >= RFID_MIN_OK) {
        return;
      }
    } else {
      n_err++;
      if (n_err >= RFID_MAX_ERRS) {
        rfid_zero_serno();
        return;
      }
    }
    _delay_ms(50);
  }
}

static void rfid_read() {
  while (1) {
    serial_write(rfid_read_cmd, sizeof(rfid_read_cmd));
    if (serial_read_blocking() == RFID_OK) {
      rfid_read_serno();
      return;
    }
  }
}

eMBErrorCode eMBRegCoilsCB(UCHAR *reg_buf, USHORT addr, USHORT num,
    eMBRegisterMode mode) {
  if (mode == MB_REG_WRITE) {
    return MB_ENOREG;
  } else if (mode == MB_REG_READ) {
    return MB_ENOREG;
  } else {
    return MB_EIO;
  }
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *reg_buf, USHORT addr, USHORT num) {
  return MB_ENOREG;
}

eMBErrorCode eMBRegInputCB(UCHAR *reg_buf, USHORT addr, USHORT num) {
  int i;
  switch (addr) {
    case 0:
      // TODO #define addresses as TYPE_NAME, eg INPUT_SERNO1
      // TODO test whether the following works as expected (8 or 16 bit buf?)
      for (i = 0; i < 2*num; i++) {
        reg_buf[i] = i;
      }
      return MB_ENOERR;
    default:
      return MB_ENOREG;
  }
}

eMBErrorCode eMBRegHoldingCB(UCHAR *reg_buf, USHORT addr, USHORT num,
    eMBRegisterMode mode) {
  if (mode == MB_REG_WRITE) {
    return MB_ENOREG;
  } else if (mode == MB_REG_READ) {
    return MB_ENOREG;
  } else {
    return MB_EIO;
  }
}

int main() {
  // init rfid's serial
  serial_init();

  // init modbus and register callbacks
  eMBInit(MB_RTU, SLAVE_ADDR, 0, MB_BAUD, MB_PAR_NONE);
  eMBEnable();

  while (1) {
    rfid_read();
    eMBPoll();
    _delay_ms(200);
  }

  return 0;
}
