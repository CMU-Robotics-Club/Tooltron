#include "current.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define CYCLES_PER_SECOND 60 // wall power
#define SAMPLES_PER_CYCLE 10
#define N_SAMPLES 20

unsigned int samples[N_SAMPLES];
int sample_idx;

unsigned int sum;
unsigned long sum_sq;

void current_init() {

  /*
   * COM0A = 0, disconnect pin
   * WGM0 = 2, clear timer on compare
   */
  TCCR0A |= _BV(WGM01);

  /*
   * CS0 = 5, 1024 prescaler
   */
  TCCR0B |= _BV(CS02) | _BV(CS00);

  OCR0A = F_CPU / 1024 / SAMPLES_PER_CYCLE / CYCLES_PER_SECOND;

  /*
   * REFS = 0, Vcc reference (set to 2 for internal 1.1V reference)
   * MUX = 8, PB3(ADC8)
   */
  ADMUX = _BV(MUX3);

  /* TODO reduce power consumption with DIDR* */

  /*
   * ADLAR = 0, right adjust result
   * ADTS = 3, start on timer 0 compare match A
   */
  ADCSRB = _BV(ADTS1) | _BV(ADTS0);

  /*
   * ADEN = 1, enable
   * ADSC = 0, don't start yet
   * ADATE = 1, auto trigger
   * ADIE = 1, enable interrupt
   * ADPS = 4, prescale clock by 16
   */
  ADCSRA |= _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2);
}

ISR(ADC_vect) {
  unsigned int old, new;

  new = ADC;

  /* put sample into ring buffer */
  old = samples[sample_idx];
  samples[sample_idx++] = new;
  if (sample_idx == N_SAMPLES)
    sample_idx = 0;

  /* keep a running total of samples and samples squared */
  sum += new;
  sum -= old;
  sum_sq += new*new;
  sum_sq -= old*old;
}

/*unsigned char isqrt(unsigned int x) {
  unsigned int sqrt, mulmask;
  sqrt = 0;
  mulmask = 0x80;
  if (x > 0) {
    while (mulmask) {
      sqrt |= mulmask;
      if (sqrt * sqrt > x)
        sqrt &= ~mulmask;
      mulmask >>= 1;
    }
  }
  return sqrt;
}*/

unsigned int current_read() {
  unsigned int _sum;
  unsigned long _sum_sq;

  cli();
  _sum = sum;
  _sum_sq = sum_sq;
  sei();

  /* calculate the variance using sum and sum_sq */
  return (N_SAMPLES*_sum_sq - _sum*_sum) / (N_SAMPLES*(N_SAMPLES-1));
}
