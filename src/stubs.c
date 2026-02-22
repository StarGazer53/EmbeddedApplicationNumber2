#include "stubs.h"

#include <stdint.h>

/*
 * Stub time source (Iteration 2).
 *
 * Why this exists:
 *  - The FPGA hardware timer/counter may not be implemented yet.
 *  - The application still needs a "current time" value for alarm logic.
 *
 * Current implementation (fake):
 *  - Keep a static counter
 *  - Each call increments the counter by 1
 *  - Return the updated value
 *
 * Future implementation (FPGA timer register):
 *  - Read TIMER_COUNTER register via MMIO
 *  - Convert ticks to seconds using a known clock frequency/prescaler
 *  - Return seconds
 */
uint32_t stub_get_fake_uptime_seconds(void)
{
  static uint32_t t = 0;
  t += 1;
  return t;
}

/*
 * Stub sensor percentage.
 *
 * Pseudocode:
 *  - Use uptime modulo 101 to create a predictable 0..100 sweep
 *  - Return that value
 */
uint32_t stub_get_fake_sensor_percent(uint32_t uptime_seconds)
{
  return (uptime_seconds % 101u);
}