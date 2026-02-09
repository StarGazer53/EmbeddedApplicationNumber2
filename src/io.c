#include "io.h"
#include "config.h"

#include <stdio.h>
#include <string.h>

#if USE_FPGA_IO
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

/* 7-seg encoding for digits 0-9 (active-low segments common on DE boards)
 * If your segments are inverted, you may need to invert these.
 */
static const uint8_t g_seven_seg_digits[10] =
{
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

static uint32_t pack_hex3_hex0(const uint8_t digits[6])
{
  /* HEX0 in lowest byte, HEX3 in highest byte of this 32-bit word */
  uint32_t v = 0;
  v |= ((uint32_t)digits[0] & 0xFFu) << 0;
  v |= ((uint32_t)digits[1] & 0xFFu) << 8;
  v |= ((uint32_t)digits[2] & 0xFFu) << 16;
  v |= ((uint32_t)digits[3] & 0xFFu) << 24;
  return v;
}

static uint32_t pack_hex5_hex4(const uint8_t digits[6])
{
  /* HEX4 in lowest byte, HEX5 in next byte */
  uint32_t v = 0;
  v |= ((uint32_t)digits[4] & 0xFFu) << 0;
  v |= ((uint32_t)digits[5] & 0xFFu) << 8;
  return v;
}

static int g_fd = -1;
static volatile uint8_t *g_lw = NULL;

static inline volatile uint32_t *reg32(uint32_t offset)
{
  return (volatile uint32_t *)(g_lw + offset);
}
#endif /* USE_FPGA_IO */

bool io_init(void)
{
#if USE_FPGA_IO
  g_fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (g_fd < 0)
  {
    perror("open(/dev/mem)");
    return false;
  }

  g_lw = (volatile uint8_t *)mmap(
    NULL,
    LW_BRIDGE_SPAN,
    PROT_READ | PROT_WRITE,
    MAP_SHARED,
    g_fd,
    LW_BRIDGE_BASE
  );

  if (g_lw == MAP_FAILED)
  {
    perror("mmap");
    close(g_fd);
    g_fd = -1;
    g_lw = NULL;
    return false;
  }

  io_hex_blank_all();
  io_write_leds(0);
  return true;
#else
  /* Stub mode needs no init */
  return true;
#endif
}

void io_close(void)
{
#if USE_FPGA_IO
  if (g_lw && g_lw != MAP_FAILED)
  {
    munmap((void *)g_lw, LW_BRIDGE_SPAN);
    g_lw = NULL;
  }
  if (g_fd >= 0)
  {
    close(g_fd);
    g_fd = -1;
  }
#endif
}

void io_read_inputs(io_inputs_t *in)
{
  if (!in) return;

#if USE_FPGA_IO
  /* Read switches + keys
   * Many DE boards have active-low keys: pressed -> 0.
   */
  in->switches = *reg32(OFS_SW);
  in->keys     = *reg32(OFS_KEY);
#else
  /* Stub inputs:
   * switches: increments like a counter
   * keys: pretend no key pressed (all 1s)
   */
  static uint32_t fake_sw = 0;
  fake_sw++;
  in->switches = fake_sw;
  in->keys = 0xFFFFFFFFu;
#endif
}

void io_write_leds(uint32_t value)
{
#if USE_FPGA_IO
  *reg32(OFS_LEDR) = value;
#else
  printf("[STUB] LEDR = 0x%08X\n", value);
#endif
}

void io_hex_write_digits(const uint8_t digits[6])
{
  if (!digits) return;

#if USE_FPGA_IO
  uint32_t v0 = pack_hex3_hex0(digits);
  uint32_t v1 = pack_hex5_hex4(digits);
  *reg32(OFS_HEX3_HEX0) = v0;
  *reg32(OFS_HEX5_HEX4) = v1;
#else
  printf("[STUB] HEX = %02X %02X %02X %02X %02X %02X\n",
         digits[5], digits[4], digits[3], digits[2], digits[1], digits[0]);
#endif
}

void io_hex_blank_all(void)
{
  uint8_t blank[6];
  for (int i = 0; i < 6; i++)
  {
    blank[i] = 0x7F; /* “all segments off” common for active-low */
  }
  io_hex_write_digits(blank);
}
