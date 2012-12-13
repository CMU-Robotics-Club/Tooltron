#ifndef LED_H
#define LED_H

#include <avr/io.h>

enum color_t {
  OFF, RED, YELLOW, GREEN
};

static inline void led_init() {DDRC |= _BV(DDC1) | _BV(DDC0);}
static inline void led_off() {PORTC &= ~(_BV(PC1) | _BV(PC0));}
static inline void led_red() {PORTC = (PORTC & ~_BV(PC0)) | _BV(PC1);}
static inline void led_yellow() {PORTC |= _BV(PC1) | _BV(PC0);}
static inline void led_green() {PORTC = (PORTC & ~_BV(PC1)) | _BV(PC0);}

/* Starts LED blinking */ 
void led_blink_start(unsigned int period_ms, char n_times, enum color_t color);

/* Returns nonzero if blinking has finished */
char led_blink_done();

/* Call this every TICK_MS to handle LED blinking */
void led_tick();

#endif
