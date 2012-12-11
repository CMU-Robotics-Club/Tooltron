#include "current.h"
#include <avr/io.h>

void current_init() {
  /*
   * REFS = 0, Vcc reference (set to 2 for internal 1.1V reference)
   * MUX = 8, PB3(ADC8)
   */
  ADMUX = _BV(MUX3);
  /* TODO reduce power consumption with DIDR* */
  /*
   * ADLAR = 0, right adjust result
   * ADTS = 0, free running mode
   */
  ADCSRB = 0;
  /*
   * ADEN = 1, enable
   * ADSC = 1, start now
   * ADATE = 1, auto trigger
   * ADIE = 0, disable interrupt
   * ADPS = 3, prescale clock by 8
   */
  ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADPS1) | _BV(ADPS0);
}

int current_read() {
  return ADC;
}
