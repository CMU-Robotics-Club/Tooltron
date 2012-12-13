#include "led.h"
#include "time.h"

char blink_count;
enum color_t blink_color;
uint16_t blink_period, blink_ticks;

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

void led_tick() {
  if (blink_count) {
    if (++blink_ticks >= blink_period) {
      blink_ticks = 0;
      if (--blink_count % 2) {
        led_color(blink_color);
      } else {
        led_off();
      }
    }
  }
}

void led_blink_start(unsigned int period_ms, char n_times, enum color_t color) {
  blink_count = n_times*2-1;
  blink_period = period_ms/2/TICK_MS;
  blink_ticks = 0;
  blink_color = color;
  led_color(color);
}

char led_blink_done() {
  return blink_count == 0;
}
