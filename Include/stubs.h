#ifndef STUBS_H
#define STUBS_H

#include <stdint.h>

uint32_t stub_get_fake_uptime_seconds(void);
uint32_t stub_get_fake_sensor_percent(uint32_t uptime_seconds);

#endif