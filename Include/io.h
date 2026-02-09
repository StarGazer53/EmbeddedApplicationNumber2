#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdbool.h>

/* Simple snapshot of inputs */
typedef struct
{
  uint32_t switches;
  uint32_t keys;      /* active-low on many DE-series boards */
} io_inputs_t;

/* IO lifecycle */
bool io_init(void);
void io_close(void);

/* Input */
void io_read_inputs(io_inputs_t *in);

/* Output */
void io_write_leds(uint32_t value);

/* 7-seg */
void io_hex_write_digits(const uint8_t digits[6]); /* digits[0]=HEX0 ... digits[5]=HEX5 */
void io_hex_blank_all(void);

#endif
