#ifndef LED_H
#define LED_H

#include <avr/io.h>

static inline void led_init() {DDRC |= _BV(DDC1) | _BV(DDC0);}
static inline void led_off() {PORTC &= ~(_BV(PC1) | _BV(PC0));}
static inline void led_red() {PORTC = (PORTC & ~_BV(PC0)) | _BV(PC1);}
static inline void led_yellow() {PORTC |= _BV(PC1) | _BV(PC0);}
static inline void led_green() {PORTC = (PORTC & ~_BV(PC1)) | _BV(PC0);}

/* Starts LED blinking */ 
void led_blink_start(unsigned int period_ms, char n_times);

/* Returns nonzero if blinking has finished */
char led_blink_done();

#endif
