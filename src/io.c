#include "io.h"

#include <stdio.h>
#include <string.h>

#if USE_FPGA_IO
  #include <fcntl.h>
  #include <sys/mman.h>
  #include <unistd.h>
  #include <errno.h>
#endif

/* =========================================================
 * Local state for MMIO
 * ========================================================= */

#if USE_FPGA_IO
static int g_mem_fd = -1;
static volatile uint8_t *g_lw_base = NULL;

static inline volatile uint32_t *reg32(uint32_t ofs)
{
  return (volatile uint32_t *)(g_lw_base + ofs);
}
#endif

/* =========================================================
 * 7-seg encoding helpers (active-low segments)
 * =========================================================
 * These values match the common DE10-Standard HEX decoding:
 *  - bit0 = segment a ... bit6 = segment g
 *  - 0 = segment ON (active-low)
 */
static uint8_t digit_to_7seg(uint8_t d)
{
  static const uint8_t map[10] = {
    0x40, /* 0 */
    0x79, /* 1 */
    0x24, /* 2 */
    0x30, /* 3 */
    0x19, /* 4 */
    0x12, /* 5 */
    0x02, /* 6 */
    0x78, /* 7 */
    0x00, /* 8 */
    0x10  /* 9 */
  };

  if (d < 10) return map[d];
  return 0x7F; /* blank */
}

bool io_init(void)
{
#if USE_FPGA_IO
  /*
   * Real /dev/mem MMIO init.
   *
   * Pseudocode:
   *  - open("/dev/mem", O_RDWR|O_SYNC)
   *  - mmap LW_BRIDGE_SPAN starting at LW_BRIDGE_BASE
   *  - store mapped base pointer
   */
  g_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (g_mem_fd < 0)
  {
    fprintf(stderr, "ERROR: open(/dev/mem) failed: %s\n", strerror(errno));
    return false;
  }

  g_lw_base = (volatile uint8_t *)mmap(NULL,
                                      LW_BRIDGE_SPAN,
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED,
                                      g_mem_fd,
                                      (off_t)LW_BRIDGE_BASE);

  if (g_lw_base == MAP_FAILED)
  {
    fprintf(stderr, "ERROR: mmap(LW bridge) failed: %s\n", strerror(errno));
    g_lw_base = NULL;
    close(g_mem_fd);
    g_mem_fd = -1;
    return false;
  }

  return true;
#else
  /* Stub mode always succeeds. */
  return true;
#endif
}

void io_close(void)
{
#if USE_FPGA_IO
  if (g_lw_base)
  {
    munmap((void *)g_lw_base, LW_BRIDGE_SPAN);
    g_lw_base = NULL;
  }
  if (g_mem_fd >= 0)
  {
    close(g_mem_fd);
    g_mem_fd = -1;
  }
#endif
}

bool io_read_inputs(io_inputs_t *in)
{
  if (!in) return false;

#if USE_FPGA_IO
  if (!g_lw_base) return false;

  in->switches = *reg32(OFS_SW);
  in->keys     = *reg32(OFS_KEY);
  return true;
#else
  /*
   * Stub input behavior.
   *
   * Pseudocode:
   *  - switches increments slowly (fake user toggling)
   *  - keys stays unpressed (all 1s for active-low)
   */
  static uint32_t fake_sw = 0;
  fake_sw = (fake_sw + 1) & 0x3;

  in->switches = fake_sw;
  in->keys     = 0xF; /* not pressed (active-low) */
  return true;
#endif
}

void io_write_leds(uint32_t ledr_value)
{
#if USE_FPGA_IO
  if (!g_lw_base) return;
  *reg32(OFS_LEDR) = ledr_value;
#else
  printf("[STUB] LEDR = 0x%08X\n", ledr_value);
#endif
}

void io_hex_blank_all(void)
{
  uint8_t hex[6] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F};
  io_hex_write_raw6(hex);
}

void io_hex_write_raw6(const uint8_t hex[6])
{
  if (!hex) return;

#if USE_FPGA_IO
  if (!g_lw_base) return;

  /* Pack HEX3..HEX0 into 32-bit register: [HEX3][HEX2][HEX1][HEX0] */
  uint32_t lo = ((uint32_t)hex[3] << 24) |
                ((uint32_t)hex[2] << 16) |
                ((uint32_t)hex[1] <<  8) |
                ((uint32_t)hex[0] <<  0);

  /* Pack HEX5..HEX4 into 32-bit register: [--][--][HEX5][HEX4] */
  uint32_t hi = ((uint32_t)hex[5] <<  8) |
                ((uint32_t)hex[4] <<  0);

  *reg32(OFS_HEX3_HEX0) = lo;
  *reg32(OFS_HEX5_HEX4) = hi;
#else
  printf("[STUB] HEX = %02X %02X %02X %02X %02X %02X\n",
         hex[5], hex[4], hex[3], hex[2], hex[1], hex[0]);
#endif
}

void io_hex_show_u32(uint32_t value)
{
  /* Show value right-aligned across HEX0..HEX5 */
  uint8_t hex[6] = {0};
  for (int i = 0; i < 6; i++)
  {
    uint8_t digit = (uint8_t)(value % 10u);
    value /= 10u;
    hex[i] = digit_to_7seg(digit);
  }
  io_hex_write_raw6(hex);
}