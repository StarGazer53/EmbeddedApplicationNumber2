#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/* =========================================================
 * Build-time mode select
 * =========================================================
 * 0 = Stub I/O (safe to run anywhere)
 * 1 = Real /dev/mem MMIO via HPS-to-FPGA Lightweight Bridge
 *
 * You can override at build time:
 *   make CFLAGS_EXTRA='-DUSE_FPGA_IO=1'
 */
#ifndef USE_FPGA_IO
#define USE_FPGA_IO 0
#endif

/* Main loop delay (milliseconds) */
#ifndef LOOP_DELAY_MS
#define LOOP_DELAY_MS 100
#endif

/* =========================================================
 * DE10-Standard Lightweight HPS-to-FPGA bridge
 * =========================================================
 * These are the common defaults for the DE10-Standard image.
 * If your Platform Designer design differs, update here.
 */
#define LW_BRIDGE_BASE   0xFF200000u
#define LW_BRIDGE_SPAN   0x00005000u

/* Common peripheral offsets (Terasic default design) */
#define OFS_LEDR         0x0000u  /* LEDR[9:0] */
#define OFS_HEX3_HEX0    0x0020u  /* HEX3-0 */
#define OFS_HEX5_HEX4    0x0030u  /* HEX5-4 */
#define OFS_SW           0x0040u  /* SW[9:0] */
#define OFS_KEY          0x0050u  /* KEY[3:0] */

#endif