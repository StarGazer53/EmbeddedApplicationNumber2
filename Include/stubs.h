#ifndef STUBS_H
#define STUBS_H

#include <stdint.h>

/* =========================================================
 * Stub “fake data” functions for Release 1
 * =========================================================
 * These simulate future hardware/sensor functionality.
 */

/* Returns a fake “sensor value” 0..100 */
uint32_t stub_get_fake_sensor_percent(void);

/* Returns a fake “uptime seconds” counter */
uint32_t stub_get_fake_uptime_seconds(void);

#endif
