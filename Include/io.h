#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

/* Input snapshot from board (or stubbed) */
typedef struct
{
  uint32_t switches; /* SW register raw */
  uint32_t keys;     /* KEY register raw (typically active-low) */
} io_inputs_t;

/* Initialize I/O layer (stub or FPGA MMIO). Returns true on success. */
bool io_init(void);

/* Shutdown / unmap any resources. Safe to call even if init failed. */
void io_close(void);

/* Read current inputs into *in. Returns true if read succeeded. */
bool io_read_inputs(io_inputs_t *in);

/* Write LEDR value (lower 10 bits used on DE10). */
void io_write_leds(uint32_t ledr_value);

/* Blank all HEX displays. */
void io_hex_blank_all(void);

/* Write HEX using raw 7-seg bytes: hex[0]=HEX0 ... hex[5]=HEX5 */
void io_hex_write_raw6(const uint8_t hex[6]);

/* Convenience: show an unsigned integer (0-999999) on HEX displays. */
void io_hex_show_u32(uint32_t value);

#endif