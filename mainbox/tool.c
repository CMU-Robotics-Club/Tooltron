#include "tool.h"
#include "query.h"
#include "tooltron_mb.h"
#include <modbus.h>
#include <stdio.h>
#include <errno.h>

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
  printf("Granting access to %08x on %s (%d)\n", tool->user, tool->name,
      tool->address);
  tool_write_coil(MB_COIL_EN, 1);
  tool->state = TS_ON;
}

static void tool_deny_access(struct tool_t *tool) {
  printf("Denying access to %08x on %s (%d)\n", tool->user, tool->name,
      tool->address);
  tool_write_coil(MB_COIL_EN, 0);
  tool->state = TS_OFF;
}

void tool_request_disable(struct tool_t *tool) {
  printf("Requesting disable on %s (%d)\n", tool->name, tool->address);
  tool_write_coil(MB_COIL_REQ_DIS, 1);
  tool->state = TS_REQ_DIS;
}

void tool_poll(struct tool_t *tool) {
  unsigned char status[N_COILS];

  if (modbus_set_slave(ctx, tool->address) == -1) {
    fprintf(stderr, "modbus_set_slave: %s\n", modbus_strerror(errno));
  }

  /* If we can't read from the tool, we only want this error message to print
   * once, thus tool->connected */
  if (modbus_read_bits(ctx, 0, N_COILS, status) == -1) {
    if (tool->connected) {
      fprintf(stderr, "Cannot connect to %s (%d): %s\n", tool->name,
          tool->address, modbus_strerror(errno));
      tool->connected = 0;
    }
    return;
  } else if (!tool->connected) {
    fprintf(stderr, "Reconnected to %s (%d)\n", tool->name, tool->address);
    tool->connected = 1;
  }

  /*uint16_t current;
  if (modbus_read_input_registers(ctx, MB_INP_CURRENT, 1, &current) == -1) {
    fprintf(stderr, "modbus_read_registers: %s\n", modbus_strerror(errno));
  } else {
    printf("Current: %d\n", current);
  }*/

  /*printf("new:%d en:%d req_dis:%d init:%d\n", status[MB_COIL_NEW],
      status[MB_COIL_EN], status[MB_COIL_REQ_DIS], status[MB_COIL_INIT]);*/

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
        if (query_user_permission(tool->address, tool->user)) {
          tool_grant_access(tool);
        } else {
          tool_deny_access(tool);
        }
      }
      break;

    case TS_ON:
      if (!status[MB_COIL_EN]) {
        printf("Tool %s (%d) is off\n", tool->name, tool->address);
        tool->state = TS_OFF;
      }
      break;

    case TS_REQ_DIS:
      if (!status[MB_COIL_EN]) {
        printf("Tool %s (%d) is off after requested disable\n", tool->name,
            tool->address);
        tool->state = TS_OFF;
      }
      break;

  }

}
