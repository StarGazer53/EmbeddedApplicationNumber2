#ifndef APP_H
#define APP_H

#include <stdint.h>
#include <stdbool.h>
#include "io.h"

typedef enum
{
  APP_MODE_IDLE = 0,
  APP_MODE_LIVE = 1,
  APP_MODE_DIAG = 2
} app_mode_t;

typedef struct
{
  app_mode_t mode;
  uint32_t loop_count;

  /* Example "rolling log" (array requirement) */
  uint32_t last_sensor_values[8];
  uint8_t  log_index;

  /* Alarm clock state */
  bool alarm_enabled;
  bool alarm_active;

  uint32_t current_time_s; /* stubbed seconds counter for now */
  uint32_t alarm_time_s;   /* target alarm time in seconds */
} app_state_t;

void app_init(app_state_t *s);
void app_step(app_state_t *s, const io_inputs_t *in);

#endif