#define _POSIX_C_SOURCE 199309L

#include "app.h"
#include "stubs.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <config.h>

static void sleep_ms(unsigned ms)
{
  struct timespec ts;
  ts.tv_sec = (time_t)(ms / 1000u);
  ts.tv_nsec = (long)((ms % 1000u) * 1000000ul);
  nanosleep(&ts, NULL);
}

static uint8_t digit_to_7seg(uint8_t d)
{
  /* Active-low segments typical; map 0..9 only, else blank */
  static const uint8_t map[10] =
  {
    0x40, 0x79, 0x24, 0x30, 0x19,
    0x12, 0x02, 0x78, 0x00, 0x10
  };

  if (d < 10) return map[d];
  return 0x7F;
}

static void show_number_on_hex(uint32_t value)
{
  /* Show last 6 digits of value on HEX0..HEX5 */
  uint8_t digits[6];

  for (int i = 0; i < 6; i++)
  {
    uint8_t d = (uint8_t)(value % 10u);
    value /= 10u;
    digits[i] = digit_to_7seg(d);
  }

  io_hex_write_digits(digits);
}

void app_init(app_state_t *s)
{
  if (!s) return;

  memset(s, 0, sizeof(*s));
  s->mode = APP_MODE_IDLE;

  io_hex_blank_all();
  io_write_leds(0);

  printf("App initialized.\n");
}

static bool key_pressed(uint32_t keys, uint8_t key_index)
{
  /* Many DE keys are active-low: 0 means pressed.
   * Bitwise requirement: mask + compare
   */
  uint32_t mask = (1u << key_index);
  return ((keys & mask) == 0u);
}

void app_step(app_state_t *s, const io_inputs_t *in)
{
  if (!s || !in) return;

  s->loop_count++;

  /* Switches: use lower 10 bits like real SW[9:0] */
  uint32_t sw10 = (in->switches & 0x3FFu); /* bitwise AND mask */

  /* KEY0 cycles modes */
  if (key_pressed(in->keys, 0))
  {
    s->mode = (app_mode_t)(((uint32_t)s->mode + 1u) % 3u);
    printf("Mode changed -> %u\n", (unsigned)s->mode);
  }

  /* Fake “sensor” + “uptime” for Release 1 */
  uint32_t sensor = stub_get_fake_sensor_percent();
  uint32_t uptime = stub_get_fake_uptime_seconds();

  /* Store into rolling log (array + pointer usage) */
  uint32_t *slot = &s->last_sensor_values[s->log_index]; /* pointer requirement */
  *slot = sensor;
  s->log_index = (uint8_t)((s->log_index + 1u) % 8u);

  if (s->mode == APP_MODE_IDLE)
  {
    /* LEDs show switches */
    io_write_leds(sw10);
    show_number_on_hex(sw10);
  }
  else if (s->mode == APP_MODE_LIVE)
  {
    /* LEDs show sensor percent + heartbeat bit */
    uint32_t heartbeat = (s->loop_count & 0x1u) << 9;  /* bitwise AND + shift */
    uint32_t led = (sensor & 0xFFu) | heartbeat;
    io_write_leds(led);

    show_number_on_hex(sensor);
    printf("LIVE: sensor=%u%% sw=%u uptime=%u\n",
           (unsigned)sensor, (unsigned)sw10, (unsigned)uptime);
  }
  else
  {
    /* DIAG: uptime on HEX, LEDs show OR of recent sensor samples */
    uint32_t or_all = 0;
    for (int i = 0; i < 8; i++)
    {
      or_all |= (s->last_sensor_values[i] & 0xFFu); /* bitwise OR */
    }

    io_write_leds(or_all);
    show_number_on_hex(uptime);

    printf("DIAG: uptime=%u sensor_or=0x%X\n",
           (unsigned)uptime, (unsigned)or_all);
  }

  sleep_ms(LOOP_DELAY_MS);
}
