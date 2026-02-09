#include "stubs.h"

/* =========================================================
 * Release 1 Stub Functions
 * ========================================================= */

/* PSEUDOCODE (stub_get_fake_sensor_percent)
 * 1) Keep an internal static counter.
 * 2) Increment it each time the function is called.
 * 3) Wrap it into 0..100 using modulo.
 * 4) Return the result as “fake sensor percent”.
 */
uint32_t stub_get_fake_sensor_percent(void)
{
  static uint32_t x = 0;
  x = (x + 7) % 101; /* deterministic “fake” pattern */
  return x;
}

/* PSEUDOCODE (stub_get_fake_uptime_seconds)
 * 1) Keep a static seconds counter.
 * 2) Increment it periodically whenever called.
 * 3) Return it as “fake uptime”.
 */
uint32_t stub_get_fake_uptime_seconds(void)
{
  static uint32_t s = 0;
  s++;
  return s;
}
