#ifndef TIME_H
#define TIME_H

#define TICK_MS 50
#define RFID_PERIOD 300

#define PRESCALE 64
#define CLOCK_SEL 3

/* Initializes timer 0 */
void time_init();

/* Waits until TICK_MS have passed since the previous call returned */
void time_wait();

#endif
