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

/* Sends the read command to the rfid reader. After, you should call rfid_poll
 * until it returns nonzero */
void rfid_start_read();

/* Should only be called after rfid_start_read. Keep calling until it returns
 * nonzero, which means it is finished reading the serial number, and
 * rfid_get_serno can be called */
char rfid_poll();

/* Attempts to read the serial number multiple times, and only accepts it if it
 * is the same every time. Don't use it */
void rfid_read_safe();

/* Call this only after rfid_poll returns nonzero. This will copy the value it
 * read into serno, which should be at least RFID_SERNO_SIZE bytes */
void rfid_get_serno(char *serno);

/* Call this only after rfid_poll returns nonzero. Returns 1 if serno matches
 * the internal buffer of the most recently read serial number */
char rfid_check_serno(char *serno);

/* Call this only after rfid_poll returns nonzero. Returns 1 if the internal
 * buffer is nonzero, meaning a serial number was successfully read from an
 * rfid tag */
char rfid_nonzero();

#endif
