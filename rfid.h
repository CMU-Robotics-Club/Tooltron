#ifndef RFID_H
#define RFID_H

/* First byte back from rfid reader */
#define RFID_OK 1

/* Bytes in serial number on rfid tags */
#define RFID_SERNO_SIZE 4

/* In rfid_read_safe, it must read the same value RFID_MIN_OK times. If it
 * encounters RFID_MAX_ERRS read errors first, it will output all 0's. */
#define RFID_MAX_ERRS 10
#define RFID_MIN_OK 5

/* Should be called before anything else */
void rfid_init();

/* Reads the serial number of an rfid tag into an internal buffer, accessbile
 * with rfid_get_serno */
void rfid_read();

/* Attempts to read the serial number multiple times, and only accepts it if it
 * is the same every time. Use rfid_read instead until this one is proven
 * necessary */
void rfid_read_safe();

/* Call this only after calling rfid_read. This will copy the value it read
 * into serno, which should be at least RFID_SERNO_SIZE bytes */
void rfid_get_serno(char *serno);

#endif
