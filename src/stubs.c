#include "stubs.h"

/* =========================================================
 * Support timing and diagnostic helpers
 * ========================================================= */

/*
 * Return a monotonically increasing uptime value.
 *
 * This function provides a deterministic software time
 * source used by the final application when a dedicated
 * hardware timer source is not being exercised.
 */
uint32_t stub_get_fake_uptime_seconds(void)
{
  static uint32_t t = 0u;
  t += 1u;
  return t;
}

/*
 * Generate a predictable diagnostic percentage value.
 *
 * The returned value cycles from 0 to 100 to support
 * logging and state validation during execution.
 */
uint32_t stub_get_fake_sensor_percent(uint32_t uptime_seconds)
{
  return (uptime_seconds % 101u);
}