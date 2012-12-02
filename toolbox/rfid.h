#ifndef RFID_H
#define RFID_H

/* Bytes in serial number on rfid tags */
#define RFID_SERNO_SIZE 4

/* Number of consective read failures before giving up and returning all 0s */
#define RFID_N_FAILURES 5

/* Should be called before anything else */
void rfid_init();

/* Sends the read command to the rfid reader. After, you should call rfid_poll
 * until it returns nonzero */
void rfid_start_read();

/* Should only be called after rfid_start_read. Keep calling until it returns
 * nonzero, which means it is finished reading the serial number, and
 * rfid_get_serno can be called */
char rfid_poll();

/* Call this only after rfid_poll returns nonzero. This will copy the value it
 * read into serno, which should be at least RFID_SERNO_SIZE bytes */
void rfid_get_serno(uint8_t *serno);

/* Call this only after rfid_poll returns nonzero. Returns 1 if the internal
 * buffer is nonzero, meaning a serial number was successfully read from an
 * rfid tag */
char rfid_nonzero();

#endif
