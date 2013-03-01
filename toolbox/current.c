#include "current.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define CYCLES_PER_SECOND 60 // wall power
#define SAMPLES_PER_CYCLE 16
#define N_SAMPLES 48

unsigned int samples[N_SAMPLES];
int sample_idx;

unsigned int sum;
unsigned long sum_sq;

void current_init() {

  /* TODO reduce power consumption with DIDR* */

  /*
   * COM1A = COM1B = 0, disconnect pins
   * WGM1 = 4, clear timer on compare A
   * CS1 = 1, no prescaler
   */
  TCCR1B = _BV(WGM12) | _BV(CS10);

  /* Timer is cleared on A, ADC is triggered on B */
  OCR1A = F_CPU / SAMPLES_PER_CYCLE / CYCLES_PER_SECOND;
  OCR1B = OCR1A;

  /*
   * REFS = 0, Vcc reference (set to 2 for internal 1.1V reference)
   * MUX = 8, PB3(ADC8)
   */
  ADMUX = _BV(MUX3);

  /*
   * ADLAR = 0, right adjust result
   * ADTS = 5, start on timer 1 compare match B
   */
  ADCSRB = _BV(ADTS2) | _BV(ADTS0);

  /*
   * ADEN = 1, enable
   * ADSC = 0, don't start yet
   * ADATE = 1, auto trigger
   * ADIE = 1, enable interrupt
   * ADPS = 6, prescale clock by 64
   */
  ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
}

ISR(ADC_vect) {
  unsigned int old, new;

  /* clear the timer interrupt flag so that the ADC will be triggered again
   * next time the timer resets */
  TIFR |= _BV(OCF1B);

  new = ADC;

  /* put sample into ring buffer */
  old = samples[sample_idx];
  samples[sample_idx++] = new;
  if (sample_idx == N_SAMPLES)
    sample_idx = 0;

  /* keep a running total of samples and samples squared */
  sum += new;
  sum -= old;
  sum_sq += (unsigned long)new*new;
  sum_sq -= (unsigned long)old*old;
}

unsigned int current_read() {
  unsigned int _sum;
  unsigned long _sum_sq;

  cli();
  _sum = sum;
  _sum_sq = sum_sq;
  sei();

  /* calculate the variance using sum and sum_sq */
  return (N_SAMPLES*_sum_sq - (unsigned long)_sum*_sum) /
    (N_SAMPLES*(N_SAMPLES-1));
}
