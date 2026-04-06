#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/* =========================================================
 * Smart Alarm Clock with FPGA Timer
 * Final Project Release - Milestone 5
 *
 * This header defines build-time configuration values,
 * DE10-Standard lightweight bridge addresses, and
 * peripheral register offsets used by the application.
 * ========================================================= */

/* =========================================================
 * Build-time mode selection
 * =========================================================
 * 0 = Stub I/O mode
 * 1 = Real FPGA MMIO mode through /dev/mem
 *
 * Example:
 *   make CFLAGS_EXTRA='-DUSE_FPGA_IO=1'
 */
#ifndef USE_FPGA_IO
#define USE_FPGA_IO 0
#endif

/* Main loop delay in milliseconds */
#ifndef LOOP_DELAY_MS
#define LOOP_DELAY_MS 100
#endif

/* Default alarm time in seconds after startup */
#ifndef DEFAULT_ALARM_TIME_S
#define DEFAULT_ALARM_TIME_S 30u
#endif

/* Size of rolling diagnostic log */
#ifndef SENSOR_LOG_SIZE
#define SENSOR_LOG_SIZE 8u
#endif

/* =========================================================
 * DE10-Standard lightweight HPS-to-FPGA bridge
 * ========================================================= */
#define LW_BRIDGE_BASE   0xFF200000u
#define LW_BRIDGE_SPAN   0x00005000u

/* Peripheral register offsets */
#define OFS_LEDR         0x0000u  /* LEDR[9:0] */
#define OFS_HEX3_HEX0    0x0020u  /* HEX3..HEX0 */
#define OFS_HEX5_HEX4    0x0030u  /* HEX5..HEX4 */
#define OFS_SW           0x0040u  /* SW[9:0] */
#define OFS_KEY          0x0050u  /* KEY[3:0] */

#endif