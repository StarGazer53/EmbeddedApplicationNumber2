/* Enable POSIX prototypes like timespec/nanosleep with -std=c99. */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "app.h"
#include "stubs.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

static void sleep_ms(unsigned ms)
{
  /*
   * Loop pacing.
   *
   * Pseudocode:
   *  - Convert milliseconds to a timespec
   *  - Call nanosleep() to pause the current thread
   *
   * Why:
   *  - Prevents the main loop from running at full speed
   *  - Makes the demo readable and reduces CPU usage
   */
  struct timespec ts;
  ts.tv_sec  = (time_t)(ms / 1000u);
  ts.tv_nsec = (long)(ms % 1000u) * 1000000L;
  (void)nanosleep(&ts, NULL);
}

void app_init(app_state_t *s)
{
  if (!s) return;
  memset(s, 0, sizeof(*s));

  s->mode = APP_MODE_IDLE;

  /* Default alarm time: 30 seconds after boot for demo */
  s->alarm_time_s = 30;

  io_write_leds(0);
  io_hex_blank_all();

  printf("App initialized.\n");
}

static bool key_is_pressed(uint32_t key_reg_raw, uint8_t key_index)
{
  /* DE10 KEYs are active-low: bit=0 means pressed */
  uint32_t mask = (1u << key_index);
  return ((key_reg_raw & mask) == 0u);
}

void app_step(app_state_t *s, const io_inputs_t *in)
{
  if (!s || !in) return;

  s->loop_count++;

  /* Alarm enable: SW0 */
  s->alarm_enabled = ((in->switches & 0x1u) != 0u);

  /* Stop/reset alarm: KEY0 */
  if (key_is_pressed(in->keys, 0))
  {
    s->alarm_active = false;
    s->mode = APP_MODE_IDLE;
  }

  /* Current time still stubbed for Iteration 2 if FPGA timer isn't ready */
  s->current_time_s = stub_get_fake_uptime_seconds();

  if (s->alarm_enabled && !s->alarm_active)
  {
    if (s->current_time_s >= s->alarm_time_s)
    {
      s->alarm_active = true;
      s->mode = APP_MODE_LIVE;
      printf("ALARM TRIGGERED at t=%u\n", s->current_time_s);
    }
  }

  /* Update outputs */
  if (s->alarm_active)
  {
    /* simple blink pattern using loop_count */
    uint32_t pattern = (s->loop_count & 1u) ? 0x3FFu : 0x000u;
    io_write_leds(pattern);
  }
  else
  {
    /* Show enabled status on LEDR0 */
    io_write_leds(s->alarm_enabled ? 0x001u : 0x000u);
  }

  /* Show time on HEX */
  io_hex_show_u32(s->current_time_s);

  /* Example rolling log requirement */
  s->last_sensor_values[s->log_index] = stub_get_fake_sensor_percent(s->current_time_s);
  s->log_index = (uint8_t)((s->log_index + 1u) & 0x7u);

  sleep_ms(LOOP_DELAY_MS);
}