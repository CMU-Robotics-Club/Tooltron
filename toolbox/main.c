#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "mb.h"
#include "mbport.h"
#include "tooltron_mb.h"
#include "rfid.h"
#include "led.h"
#include "current.h"
#include "time.h"

#if TOOL_ADDRESS < 0
#error Please define TOOL_ADDRESS
#endif

enum toolstate_t {
  TS_INIT,
  TS_OFF,
  TS_WAIT_ACCESS,
  TS_DENY,
  TS_REQ_DIS,
  TS_MISSING_ID,
  TS_ON,
  TS_OVER_CURRENT
};

static enum toolstate_t toolstate = TS_INIT;
static uint8_t coils;
static uint8_t latest_reading[RFID_SERNO_SIZE];
static uint8_t current_user[RFID_SERNO_SIZE];
static uint16_t current;
//static uint16_t current_max_warn, current_max_hard;

// watch for current spike when tool turns on
#define CURRENT_STARTUP_THRESH   10
#define CURRENT_STARTUP_TIMEOUT 200 // ms
static uint8_t current_startup_timeout;
static uint16_t current_startup_value;

static inline void set_coil(char coil, char bit) {
  coils = (coils & ~(1 << coil)) | (bit << coil);
}
static inline char get_coil(char coil) {
  return (coils >> coil) & 1;
}

static inline void tool_init() {DDRA |= _BV(DDA1);}
static inline void tool_enable() {PORTA |= _BV(PA1);}
static inline void tool_disable() {PORTA &= ~ _BV(PA1);}

static inline void serno_zero(uint8_t *serno) {
  memset(serno, 0, RFID_SERNO_SIZE);
}

static char serno_is_nonzero(uint8_t *serno) {
  int i;
  for (i = 0; i < RFID_SERNO_SIZE; i++) {
    if (serno[i]) {
      return 1;
    }
  }
  return 0;
}

static char serno_equal(uint8_t *a, uint8_t *b) {
  return memcmp(a, b, RFID_SERNO_SIZE) == 0;
}

static void serno_cpy(uint8_t *dest, uint8_t *src) {
  memcpy(dest, src, RFID_SERNO_SIZE);
}

static void tool_tick() {

  switch (toolstate) {

    case TS_INIT:
      if (get_coil(MB_COIL_INIT)) {
        set_coil(MB_COIL_NEW, 0);
        set_coil(MB_COIL_EN, 0);
        set_coil(MB_COIL_REQ_DIS, 0);
        toolstate = TS_OFF;
      }
      break;

    case TS_OFF:
      led_off();
      set_coil(MB_COIL_EN, 0);
      if (serno_is_nonzero(latest_reading)) {
        serno_cpy(current_user, latest_reading);
        set_coil(MB_COIL_NEW, 1);
        toolstate = TS_WAIT_ACCESS;
      }
      break;

    case TS_WAIT_ACCESS:
      led_yellow();
      if (get_coil(MB_COIL_EN)) {
        tool_enable();
        toolstate = TS_ON;
        current_startup_timeout = CURRENT_STARTUP_TIMEOUT / TICK_MS;
        current_startup_value = current + CURRENT_STARTUP_THRESH;
      } else if (!get_coil(MB_COIL_NEW)) {
        toolstate = TS_DENY;
      } else if (!serno_equal(current_user, latest_reading)) {
        set_coil(MB_COIL_NEW, 0);
        toolstate = TS_OFF;
      }
      break;

    case TS_DENY:
      led_red();
      if (!serno_equal(current_user, latest_reading)) {
        toolstate = TS_OFF;
        serno_zero(current_user);
      }
      break;

    case TS_REQ_DIS:
      if (!get_coil(MB_COIL_EN)) {
        tool_disable();
        toolstate = TS_OFF;
      } else if (!get_coil(MB_COIL_REQ_DIS)) {
        toolstate = TS_ON;
      } else {
        // TODO blink yellow for 10 seconds or something
        set_coil(MB_COIL_EN, 0);
        set_coil(MB_COIL_REQ_DIS, 0);
        tool_disable();
        serno_zero(current_user);
        toolstate = TS_OFF;
      }
      break;

    case TS_MISSING_ID:
      if (current_startup_timeout > 0 && current > current_startup_value) {
        tool_disable();
        set_coil(MB_COIL_EN, 0);
        toolstate = TS_OVER_CURRENT;
        led_blink_start(500, 16, RED);
      } else if (!get_coil(MB_COIL_EN)) {
        tool_disable();
        toolstate = TS_OFF;
      } else if (get_coil(MB_COIL_REQ_DIS)) {
        toolstate = TS_REQ_DIS;
      } else if (serno_equal(current_user, latest_reading)) {
        toolstate = TS_ON;
      } else if (led_blink_done()) {
        set_coil(MB_COIL_EN, 0);
        tool_disable();
        serno_zero(current_user);
        toolstate = TS_OFF;
      }
      if (current_startup_timeout > 0) {
        current_startup_timeout--;
      }
      break;

    case TS_ON:
      led_green();
      if (current_startup_timeout > 0 && current > current_startup_value) {
        tool_disable();
        set_coil(MB_COIL_EN, 0);
        toolstate = TS_OVER_CURRENT;
        led_blink_start(500, 16, RED);
      } else if (!get_coil(MB_COIL_EN)) {
        tool_disable();
        serno_zero(current_user);
        toolstate = TS_OFF;
      } else if(get_coil(MB_COIL_REQ_DIS)) {
        toolstate = TS_REQ_DIS;
      } else if (!serno_equal(current_user, latest_reading)) {
        toolstate = TS_MISSING_ID;
        led_blink_start(500, 16, YELLOW);
      }
      if (current_startup_timeout > 0) {
        current_startup_timeout--;
      }
      break;

    case TS_OVER_CURRENT:
      if (led_blink_done() && !serno_equal(current_user, latest_reading)) {
        toolstate = TS_OFF;
        serno_zero(current_user);
      }
      break;

  }

}

eMBErrorCode eMBRegCoilsCB(UCHAR *reg_buf, USHORT addr, USHORT n_coils,
    eMBRegisterMode mode) {

  addr--;

  if (addr+n_coils > N_COILS) {
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
        set_coil(MB_COIL_REQ_DIS, reg_buf[0] & 1);
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

  addr--;

  switch (addr) {

    case MB_INP_SERNOL:
      *reg_buf++ = current_user[0];
      *reg_buf++ = current_user[1];
      n_regs--;
      if (n_regs == 0) {
        return MB_ENOERR;
      }

    case MB_INP_SERNOH:
      *reg_buf++ = current_user[2];
      *reg_buf++ = current_user[3];
      n_regs--;
      if (n_regs == 0) {
        return MB_ENOERR;
      }

    case MB_INP_CURRENT:
      *reg_buf++ = (uint8_t)(current >> 8);
      *reg_buf++ = (uint8_t)current;
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
  char rfid_ticks = 0;

  time_init();
  led_init();
  tool_init();
  rfid_init();
  current_init();

  eMBInit(MB_RTU, TOOL_ADDRESS, 0, MB_BAUD, MB_PAR_NONE);
  eMBEnable();

  sei();

  rfid_start_read();
  while (1) {
    if (rfid_poll()) {
      rfid_get_serno(latest_reading);
    }
    if (++rfid_ticks >= RFID_PERIOD/TICK_MS)
    {
      rfid_ticks = 0;
      rfid_start_read();
    }
    current = current_read();
    tool_tick();
    led_tick();
    eMBPoll();
    time_wait();
  }

  return 0;
}
