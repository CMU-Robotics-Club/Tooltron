#include "serial.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static int rx_start = 0, rx_end = 0;
static char rx_buffer[RX_BUFFER_SIZE];

ISR(USART1_RX_vect) {
  char data = UDR1;
  int new_end = rx_end+1;
  if (new_end == RX_BUFFER_SIZE) {
    new_end = 0;
  }
  if (new_end == rx_start) {
    // TODO warn of buffer overflow?
  } else {
    rx_buffer[rx_end] = data;
    rx_end = new_end;
  }
}

void serial_init() {
  // baud = F_CPU / (8 (UBRR + 1))
  uint16_t ubrr = F_CPU / 8 / BAUD_RATE - 1;
  UBRR1H = ubrr >> 8;
  UBRR1L = ubrr;
  // UMSEL = 0, asynchronous usart
  // UPM = 0, parity check disabled
  // USBS = 0, 1 stop bit
  // UCSZ = 3, 8-bit
  // U2X = 1, use 8 prescale instead of 16 for a more accurate baud rate
  UCSR1B = _BV(RXCIE1) | _BV(RXEN1) | _BV(TXEN1) | _BV(U2X1);
  UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);
}

int serial_read() {
  int ret;
  cli();
  if (rx_start == rx_end) {
    ret = -1;
  } else {
    ret = rx_buffer[rx_start];
    rx_start++;
    if (rx_start == RX_BUFFER_SIZE)
      rx_start = 0;
  }
  sei();
  return ret;
}

char serial_read_blocking() {
  int c;
  do {
    c = serial_read();
  } while (c < 0);
  return c;
}

void serial_write(char* data, int length) {
  int i;
  for (i = 0; i < length; i++) {
    while (!(UCSR1A & _BV(UDRE1)));
    UDR1 = data[i];
  }
}
