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

/* When 1, there is a new rfid tag in the tool. The main box should either
 * write 1 to MB_COIL_EN, which automatically clears MB_COIL_NEW, or it should
 * write 0 directly to MB_COIL_NEW which denies the request */
#define MB_COIL_NEW     0

/* When 1, tool can be turned on. Write 0 to immediately shut off tool */
#define MB_COIL_EN      1

/* Write 1 to request that the tool is disabled soon (what that means will be
 * determined later) */
#define MB_COIL_REQ_DIS 2

/* When 1, tool has been properly initialized. When 0, tool has lost power and
 * the main box should rewrite values for current limits (to be implemented
 * later) */
#define MB_COIL_INIT    3

/*
 * Other things
 */
#define SLAVE_ADDR 4 // TODO move to makefile?
#define MB_BAUD 9600

#endif
