#ifndef SERIAL_H
#define SERIAL_H

#define BAUD_RATE 9600

#define RX_BUFFER_SIZE 256

void serial_init();
int serial_read();
void serial_flush();
char serial_read_blocking();
void serial_write(char* data, int length);

#endif
