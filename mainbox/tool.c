#include "tool.h"
#include "query.h"
#include "event.h"
#include "log.h"
#include "tooltron_mb.h"
#include "cache.h"
#include <modbus.h>
#include <stdio.h>
#include <errno.h>

modbus_t *ctx;

int tool_init_mb(const char *device) {
  struct timeval timeout;

  ctx = modbus_new_rtu(device, MB_BAUD, 'N', 8, 1);
  if (ctx == NULL) {
    log_print("ERROR: modbus_new_rtu: %s", modbus_strerror(errno));
    return 1;
  }

  if (modbus_connect(ctx) == -1) {
    log_print("ERROR: modbus_connect: %s", modbus_strerror(errno));
    modbus_free(ctx);
    return 1;
  }

  timeout.tv_sec = 0;
  timeout.tv_usec = MB_TIMEOUT_MS * 1000;
  modbus_set_response_timeout(ctx, &timeout);

  modbus_get_response_timeout(ctx, &timeout);
  log_print("Modbus response timeout is %lus %luus",
      timeout.tv_sec, timeout.tv_usec);

  return 0;
}

void tool_close_mb() {
  modbus_close(ctx);
  modbus_free(ctx);
}

static void tool_write_coil(int addr, int bit) {
  if (modbus_write_bit(ctx, addr, bit) == -1) {
    log_print("ERROR: modbus_write_bit: %s", modbus_strerror(errno));
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
    log_print("ERROR: modbus_read_registers: %s", modbus_strerror(errno));
    tool->user = 0;
  } else {
    tool->user = MODBUS_GET_INT32_FROM_INT16(serno, 0);
  }
}

static void tool_grant_access(struct tool_t *tool) {

  log_print("Granting access to %08x on %s (%d)", tool->user, tool->name,
      tool->address);

  tool_write_coil(MB_COIL_EN, 1);
  tool->state = TS_ON;

  tool->event = event_alloc();
  tool->event->user = tool->user;
  tool->event->tool_id = tool->address;
  tool->event->tstart = time(NULL);
  tool->event->succ = 1;
}

static void tool_deny_access(struct tool_t *tool) {
  struct event_t *event;

  log_print("Denying access to %08x on %s (%d)", tool->user, tool->name,
      tool->address);

  tool_write_coil(MB_COIL_EN, 0);
  tool->state = TS_OFF;

  event = event_alloc();
  event->user = tool->user;
  event->tool_id = tool->address;
  event->tstart = time(NULL);
  event->tend = event->tstart;
  event->succ = 0;
  event_q_push(event);
}

static void tool_off(struct tool_t *tool) {
  tool->state = TS_OFF;
  tool->event->tend = time(NULL);
  event_q_push(tool->event);
  tool->event = NULL;
}

void tool_request_disable(struct tool_t *tool) {
  if (tool->state == TS_ON) {
    log_print("Requesting disable on %s (%d)", tool->name, tool->address);
    tool_write_coil(MB_COIL_REQ_DIS, 1);
    tool->state = TS_REQ_DIS;
  }
}

void tool_poll(struct tool_t *tool) {
  unsigned char status[N_COILS];

  if (modbus_set_slave(ctx, tool->address) == -1) {
    log_print("ERROR: modbus_set_slave: %s", modbus_strerror(errno));
  }

  /* If we can't read from the tool, we only want this error message to print
   * once, thus tool->connected */
  if (modbus_read_bits(ctx, 0, N_COILS, status) == -1) {
    if (tool->connected) {
      log_print("Cannot connect to %s (%d): %s", tool->name,
          tool->address, modbus_strerror(errno));
      tool->connected = 0;
    }
    return;
  } else if (!tool->connected) {
    log_print("Reconnected to %s (%d)", tool->name, tool->address);
    tool->connected = 1;
  }

  /*uint16_t current;
  if (modbus_read_input_registers(ctx, MB_INP_CURRENT, 1, &current) == -1) {
    log_print("ERROR: modbus_read_registers: %s", modbus_strerror(errno));
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

        int permission = query_user_permission(tool->address, tool->user);

        // If network error check the cache
        if (permission == -1) {
          unsigned int v;
          if(cache_lookup(tool->user, tool->address, &v)) {
            if(v)
              tool_grant_access(tool);
            else
              tool_deny_access(tool);
          } else {
            tool_deny_access(tool);
          }
        } else {
          if(permission) {
            tool_grant_access(tool);
          } else {
            tool_deny_access(tool);
          }

          cache_update(tool->user, tool->address, permission);
        }
      }
      break;

    case TS_ON:
      if (!status[MB_COIL_EN]) {
        log_print("Tool %s (%d) is off", tool->name, tool->address);
        tool_off(tool);
      }
      break;

    case TS_REQ_DIS:
      if (!status[MB_COIL_EN]) {
        log_print("Tool %s (%d) is off after requested disable", tool->name,
            tool->address);
        tool_off(tool);
      }
      break;

  }

}
