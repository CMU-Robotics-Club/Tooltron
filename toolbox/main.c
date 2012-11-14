#include <stdint.h>
#include <util/delay.h>
#include "mb.h"
#include "mbport.h"
#include "tooltron_mb.h"
#include "rfid.h"

static char coils;

static inline void set_coil(char coil, char bit) {
  coils |= (bit << coil);
}

eMBErrorCode eMBRegCoilsCB(UCHAR *reg_buf, USHORT addr, USHORT n_coils,
    eMBRegisterMode mode) {

  if (addr >= N_COILS || n_coils >= N_COILS-addr) {
    return MB_ENOREG;
  }

  if (mode == MB_REG_WRITE) {

    switch (addr) {

      case MB_COIL_NEW:
        /* nop */
        reg_buf[0] >>= 1;
        n_coils--;
        if (n_coils == 0) {
          return MB_ENOERR;
        }

      case MB_COIL_EN:
        set_coil(MB_COIL_NEW, 0);
        set_coil(MB_COIL_EN, reg_buf[0] & 1);
        reg_buf[0] >>= 1;
        n_coils--;
        if (n_coils == 0) {
          return MB_ENOERR;
        }

      case MB_COIL_REQ_DIS:
        set_coil(MB_COIL_INIT, reg_buf[0] & 1);
        reg_buf[0] >>= 1;
        n_coils--;
        if (n_coils == 0) {
          return MB_ENOERR;
        }

      case MB_COIL_INIT:
        set_coil(MB_COIL_INIT, reg_buf[0] & 1);
        reg_buf[0] >>= 1;
        n_coils--;
        if (n_coils == 0) {
          return MB_ENOERR;
        }
    }

  } else if (mode == MB_REG_READ) {

    reg_buf[0] = (coils >> addr) & ((1 << n_coils) - 1);
    return MB_ENOERR;

  }

  return MB_EIO;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *reg_buf, USHORT addr, USHORT n_coils) {
  return MB_ENOREG;
}

eMBErrorCode eMBRegInputCB(UCHAR *reg_buf, USHORT addr, USHORT n_regs) {
  char serno[RFID_SERNO_SIZE];

  rfid_get_serno(serno);

  switch (addr) {

    case MB_INP_SERNOL:
      // TODO check that these (and the ones in SERNOH) are in the right order
      *reg_buf++ = serno[0];
      *reg_buf++ = serno[1];
      n_regs--;
      if (n_regs == 0) {
        return MB_ENOERR;
      }

    case MB_INP_SERNOH:
      *reg_buf++ = serno[2];
      *reg_buf++ = serno[3];
      n_regs--;
      if (n_regs == 0) {
        return MB_ENOERR;
      }

    case MB_INP_CURRENT:
      *reg_buf++ = 0;
      *reg_buf++ = 0;
      n_regs--;
      if (n_regs == 0) {
        return MB_ENOERR;
      }

    default:
      return MB_ENOREG;
  }
}

eMBErrorCode eMBRegHoldingCB(UCHAR *reg_buf, USHORT addr, USHORT n_regs,
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
  rfid_init();

  eMBInit(MB_RTU, SLAVE_ADDR, 0, MB_BAUD, MB_PAR_NONE);
  eMBEnable();

  /* Set pin controlling relay to output */
  DDRA |= _BV(DDA1);

  sei();

  while (1) {
    rfid_read();
    /* TODO act on rfid input and coil state */
    eMBPoll();
    _delay_ms(200);
  }

  return 0;
}
