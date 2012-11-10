#include <stdint.h>
#include <util/delay.h>
#include "mb.h"
#include "mbport.h"
#include "tooltron_mb.h"
#include "rfid.h"

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
