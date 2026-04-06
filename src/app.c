#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include "app.h"
#include "stubs.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/* =========================================================
 * Local helpers
 * ========================================================= */

/*
 * Pause the loop for the specified number of milliseconds.
 */
static void sleep_ms(unsigned ms)
{
  struct timespec ts;
  ts.tv_sec = (time_t)(ms / 1000u);
  ts.tv_nsec = (long)(ms % 1000u) * 1000000L;
  (void)nanosleep(&ts, NULL);
}

/*
 * Return true when the selected key is pressed.
 *
 * DE10 push-buttons are active-low, so a value of 0 means
 * the button is pressed.
 */
static bool key_is_pressed(uint32_t key_reg_raw, uint8_t key_index)
{
  uint32_t mask = (1u << key_index);
  return ((key_reg_raw & mask) == 0u);
}

/* =========================================================
 * Public application functions
 * ========================================================= */

/*
 * Initialize the application state and outputs.
 */
void app_init(app_state_t *s)
{
  if (!s)
  {
    return;
  }

  memset(s, 0, sizeof(*s));

  s->mode = APP_MODE_IDLE;
  s->alarm_time_s = DEFAULT_ALARM_TIME_S;

  io_write_leds(0u);
  io_hex_blank_all();

  printf("Application initialized.\n");
  printf("Default alarm time: %u seconds\n", s->alarm_time_s);
}

/*
 * Execute one control-loop iteration.
 */
void app_step(app_state_t *s, const io_inputs_t *in)
{
  if (!s || !in)
  {
    return;
  }

  s->loop_count++;

  /* SW0 enables or disables the alarm function. */
  s->alarm_enabled = ((in->switches & 0x1u) != 0u);

  /* KEY0 clears an active alarm and returns to idle mode. */
  if (key_is_pressed(in->keys, 0u))
  {
    s->alarm_active = false;
    s->mode = APP_MODE_IDLE;
  }

  /* Obtain the current time source for alarm comparison. */
  s->current_time_s = stub_get_fake_uptime_seconds();

  /* Trigger the alarm once the time threshold is reached. */
  if (s->alarm_enabled && !s->alarm_active)
  {
    if (s->current_time_s >= s->alarm_time_s)
    {
      s->alarm_active = true;
      s->mode = APP_MODE_LIVE;
      printf("ALARM TRIGGERED at t=%u\n", s->current_time_s);
    }
  }

  /* Update LEDs to reflect current system state. */
  if (s->alarm_active)
  {
    uint32_t pattern = (s->loop_count & 1u) ? 0x3FFu : 0x000u;
    io_write_leds(pattern);
  }
  else
  {
    io_write_leds(s->alarm_enabled ? 0x001u : 0x000u);
  }

  /* Show the current time on the HEX displays. */
  io_hex_show_u32(s->current_time_s);

  /* Store a rolling diagnostic value for validation/logging. */
  s->last_sensor_values[s->log_index] =
      stub_get_fake_sensor_percent(s->current_time_s);

  s->log_index = (uint8_t)((s->log_index + 1u) % SENSOR_LOG_SIZE);

  sleep_ms(LOOP_DELAY_MS);
}