#ifndef APP_H
#define APP_H

#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "config.h"

/* =========================================================
 * Application state and public interface
 * ========================================================= */

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

  /* Rolling diagnostic log */
  uint32_t last_sensor_values[SENSOR_LOG_SIZE];
  uint8_t log_index;

  /* Alarm state */
  bool alarm_enabled;
  bool alarm_active;

  /* Time tracking */
  uint32_t current_time_s;
  uint32_t alarm_time_s;
} app_state_t;

/* Initialize the application state. */
void app_init(app_state_t *s);

/* Execute one application loop iteration. */
void app_step(app_state_t *s, const io_inputs_t *in);

#endif