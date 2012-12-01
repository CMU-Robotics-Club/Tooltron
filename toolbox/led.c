#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define PRESCALE 64
#define CLOCK_SEL 3

/* F_CPU / PRESCALE = OCR * 1000 + ERROR */
#define OCR (F_CPU / PRESCALE / 1000UL)
#define ERROR (F_CPU / PRESCALE - OCR * 1000UL)

char count;
uint16_t period;

uint16_t ms;
uint16_t error;

static void blink() {
  count--;
  if (count % 2) {
    led_yellow();
  } else {
    led_off();
  }
}

ISR(TIMER0_COMPA_vect) {
  error += ERROR;
  if (error >= 1000) {
    error -= 1000;
  } else {
    ms++;
    if (ms == period) {
      blink();
      if (count == 0) {
        TCCR0B = 0;
      }
      ms = 0;
    }
  }
}

void led_blink_start(unsigned int period_ms, char n_times) {
  led_yellow();
  ms = 0;
  error = 0;
  count = n_times*2-1;
  period = period_ms/2;
  OCR0A = OCR;
  TIMSK = _BV(OCIE0A);
  TCCR0A = _BV(WGM01);
  TCCR0B = CLOCK_SEL;
}

char led_blink_done() {
  return count == 0;
}
