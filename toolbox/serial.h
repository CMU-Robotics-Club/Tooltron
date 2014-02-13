#ifndef SERIAL_H
#define SERIAL_H

#define BAUD_RATE 19200

#define RX_BUFFER_SIZE 64

void serial_init();
int serial_read();
void serial_flush();
uint8_t serial_read_blocking();
void serial_write(uint8_t *data, int length);

#endif
