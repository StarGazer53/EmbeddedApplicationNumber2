#ifndef STUBS_H
#define STUBS_H

#include <stdint.h>

/* =========================================================
 * Support services for non-hardware timing and diagnostics
 *
 * These functions allow the application to run in a
 * repeatable way when a dedicated FPGA timer source is
 * unavailable during development or demonstration.
 * ========================================================= */

/* Return a monotonically increasing uptime value in seconds. */
uint32_t stub_get_fake_uptime_seconds(void);

/* Return a predictable diagnostic sensor percentage. */
uint32_t stub_get_fake_sensor_percent(uint32_t uptime_seconds);

#endif