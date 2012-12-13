#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "time.h"

/* F_CPU / PRESCALE = OCR * 1000 + ERROR */
#define OCR (F_CPU / PRESCALE / 1000UL)
#define ERROR (F_CPU / PRESCALE - OCR * 1000UL)

static volatile char ready;
static char ms;
static uint16_t error;

ISR(TIMER0_COMPA_vect) {
  error += ERROR;
  if (error >= 1000) {
    error -= 1000;
    OCR0A = OCR+1;
  } else {
    OCR0A = OCR;
  }
  if (++ms == TICK_MS) {
    ready = 1;
    ms = 0;
  }
}

void time_init() {
  OCR0A = OCR;
  TIMSK = _BV(OCIE0A);
  TCCR0A = _BV(WGM01);
  TCCR0B = CLOCK_SEL;
}

void time_wait() {
  while (!ready);
  ready = 0;
}
