#include "tool.h"
#include "tooltron_mb.h"
#include <modbus.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h> // TODO temporary

modbus_t *ctx;

int tool_init_mb(const char *device) {
  struct timeval timeout;

  ctx = modbus_new_rtu(device, MB_BAUD, 'N', 8, 1);
  if (ctx == NULL) {
    fprintf(stderr, "modbus_new_rtu: %s\n", modbus_strerror(errno));
    return 1;
  }

  if (modbus_connect(ctx) == -1) {
    fprintf(stderr, "modbus_connect: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return 1;
  }

  modbus_get_response_timeout(ctx, &timeout);
  printf("Response timeout is %lus %luus\n", timeout.tv_sec, timeout.tv_usec);

  return 0;
}

void tool_close_mb() {
  modbus_close(ctx);
  modbus_free(ctx);
}

static void tool_write_coil(int addr, int bit) {
  if (modbus_write_bit(ctx, addr, bit) == -1) {
    fprintf(stderr, "modbus_write_bit: %s\n", modbus_strerror(errno));
  }
}

static void tool_init(struct tool_t *tool) {
  // TODO write current limit values
  tool_write_coil(MB_COIL_INIT, 1);
  tool->state = TS_OFF;
}

static void tool_read_user(struct tool_t *tool) {
  unsigned short serno[2];
  if (modbus_read_input_registers(ctx, MB_INP_SERNOL, 2, serno) == -1) {
    fprintf(stderr, "modbus_read_registers: %s\n", modbus_strerror(errno));
    tool->user = 0;
  } else {
    tool->user = MODBUS_GET_INT32_FROM_INT16(serno, 0);
  }
}

static void tool_grant_access(struct tool_t *tool) {
  printf("Granting access to %08x on %s\n", tool->user, tool->name);
  tool_write_coil(MB_COIL_EN, 1);
}

static void tool_deny_access(struct tool_t *tool) {
  printf("Denying access to %08x on %s\n", tool->user, tool->name);
  tool_write_coil(MB_COIL_EN, 0);
}

void tool_request_disable(struct tool_t *tool) {
  printf("Requesting disable on %s\n", tool->name);
  tool_write_coil(MB_COIL_REQ_DIS, 1);
}

void tool_poll(struct tool_t *tool) {
  unsigned char status[N_COILS];

  if (modbus_set_slave(ctx, tool->address) == -1) {
    fprintf(stderr, "modbus_set_slave: %s\n", modbus_strerror(errno));
  }

  tool_read_user(tool);
  printf("%08x\n", tool->user);
  return;

  if (modbus_read_bits(ctx, 0, N_COILS, status) == -1) {
    fprintf(stderr, "modbus_read_bits: %s\n", modbus_strerror(errno));
    return;
  }

  if (!status[MB_COIL_INIT]) {
    tool->state = TS_INIT;
  }

  switch (tool->state) {

    case TS_INIT:
      tool_init(tool);
      break;

    case TS_OFF:
      if (status[MB_COIL_NEW]) {
        tool_read_user(tool);
        // TODO check actual credentials
        if (rand() & 1) {
          tool_grant_access(tool);
        } else {
          tool_deny_access(tool);
        }
      }
      break;

    case TS_ON:
      if (!status[MB_COIL_EN]) {
        printf("Tool %s is off\n", tool->name);
        tool->state = TS_OFF;
      }
      break;

    case TS_REQ_DIS:
      if (!status[MB_COIL_EN]) {
        printf("Tool %s is off after requested disable\n", tool->name);
        tool->state = TS_OFF;
      }
      break;

  }

}
