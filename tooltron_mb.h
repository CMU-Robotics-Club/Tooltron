#ifndef TOOLTRON_MB_H
#define TOOLTRON_MB_H

/*
 * Input registers, 16 bit read only
 */

/* Serial number from rfid reader, or 0 if there is no rfid tag present */
#define MB_INP_SERNOL  0
#define MB_INP_SERNOH  1

/* Value from current sensor */
#define MB_INP_CURRENT 2

/*
 * Coils, 1 bit read/write
 */

/* When 1, there is a new rfid tag in the tool. Automatically cleared when the
 * main box writes to MB_COIL_EN */
#define MB_COIL_NEW     0

/* When 1, the tool receives power. Write 1 or 0 to grant or deny an access
 * request, respectively. If the tool is running, write 0 to immediately shut
 * off the tool */
#define MB_COIL_EN      1

/* Write 1 to request that the tool is disabled soon (what that means will be
 * determined later) */
#define MB_COIL_REQ_DIS 2

/* When 1, tool has been properly initialized. When 0, tool has lost power and
 * the main box should rewrite values for current limits (to be implemented
 * later) */
#define MB_COIL_INIT    3

/* Number of coils */
#define N_COILS 4

/*
 * Other things
 */
#define SLAVE_ADDR 4 // TODO move to makefile?
#define MB_BAUD 9600

#endif
