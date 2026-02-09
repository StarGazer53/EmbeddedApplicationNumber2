#ifndef CONFIG_H
#define CONFIG_H

/* =========================================================
 * Project Configuration
 * =========================================================
 * Set USE_FPGA_IO to 0 for stub mode (safe anywhere).
 * Set USE_FPGA_IO to 1 to use /dev/mem MMIO on the DE10.
 */
#define USE_FPGA_IO 0

/* Main loop delay (milliseconds) */
#define LOOP_DELAY_MS 100

/* =========================================================
 * HPS-to-FPGA Lightweight Bridge (common default)
 * =========================================================
 * NOTE: These addresses/offsets can vary depending on the
 * Qsys/Platform Designer design you are using.
 * If your board image uses different offsets, update them here.
 */
#define LW_BRIDGE_BASE   0xFF200000u
#define LW_BRIDGE_SPAN   0x00005000u

/* Common “typical” offsets (adjust if needed) */
#define OFS_LEDR         0x0000u  /* LEDs register */
#define OFS_HEX3_HEX0    0x0020u  /* HEX3-0 register */
#define OFS_HEX5_HEX4    0x0030u  /* HEX5-4 register */
#define OFS_SW           0x0040u  /* Switches register */
#define OFS_KEY          0x0050u  /* Push-buttons register */

#endif
