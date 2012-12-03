#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define PRESCALE 64
#define CLOCK_SEL 3

/* F_CPU / PRESCALE = OCR * 1000 + ERROR */
#define OCR (F_CPU / PRESCALE / 1000UL)
#define ERROR (F_CPU / PRESCALE - OCR * 1000UL)

char blink_count;
enum color_t blink_color;
uint16_t blink_period;

uint16_t ms;
uint16_t error;

static void led_color(enum color_t color) {
  switch (color) {
    case OFF:
      led_off();
      break;
    case RED:
      led_red();
      break;
    case YELLOW:
      led_yellow();
      break;
    case GREEN:
      led_green();
      break;
  }
}

static void blink() {
  blink_count--;
  if (blink_count % 2) {
    led_color(blink_color);
  } else {
    led_off();
  }
}

ISR(TIMER0_COMPA_vect) {
  error += ERROR;
  if (error >= 1000) {
    error -= 1000;
    OCR0A = OCR+1;
  } else {
    OCR0A = OCR;
  }
  ms++;
  if (ms == blink_period) {
    blink();
    if (blink_count == 0) {
      TCCR0B = 0;
    }
    ms = 0;
  }
}

void led_blink_start(unsigned int period_ms, char n_times, enum color_t color) {

  ms = 0;
  error = 0;

  blink_count = n_times*2-1;
  blink_period = period_ms/2;
  blink_color = color;
  led_color(color);

  OCR0A = OCR;
  TIMSK = _BV(OCIE0A);
  TCCR0A = _BV(WGM01);
  TCCR0B = CLOCK_SEL;

}

char led_blink_done() {
  return blink_count == 0;
}
