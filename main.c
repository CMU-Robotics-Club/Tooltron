#include <stdint.h>
#include <util/delay.h>
#include "mb.h"
#include "mbport.h"
#include "tooltron_mb.h"
#include "rfid.h"

eMBErrorCode eMBRegCoilsCB(UCHAR *reg_buf, USHORT addr, USHORT n_coils,
    eMBRegisterMode mode) {
  // TODO implement coils
  if (mode == MB_REG_WRITE) {
    return MB_ENOREG;
  } else if (mode == MB_REG_READ) {
    return MB_ENOREG;
  } else {
    return MB_EIO;
  }
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

  while (1) {
    rfid_read();
    eMBPoll();
    _delay_ms(200);
  }

  return 0;
}
