#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

/* =========================================================
 * I/O abstraction layer
 *
 * This module provides a consistent interface for reading
 * inputs and writing outputs in either:
 *  - stub mode
 *  - FPGA MMIO mode
 * ========================================================= */

typedef struct
{
  uint32_t switches;
  uint32_t keys;
} io_inputs_t;

/* Initialize the I/O subsystem. */
bool io_init(void);

/* Release any allocated I/O resources. */
void io_close(void);

/* Read the current input snapshot. */
bool io_read_inputs(io_inputs_t *in);

/* Write a value to the LEDR output register. */
void io_write_leds(uint32_t ledr_value);

/* Blank all six HEX displays. */
void io_hex_blank_all(void);

/* Write raw 7-segment bytes to HEX0..HEX5. */
void io_hex_write_raw6(const uint8_t hex[6]);

/* Display an unsigned integer on HEX0..HEX5. */
void io_hex_show_u32(uint32_t value);

#endif