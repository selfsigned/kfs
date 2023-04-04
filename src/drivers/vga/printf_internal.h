#ifndef PRINTF_INTERNAL_H
#define PRINTF_INTERNAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct printf_flags {
  bool left_justify; // '-'
  bool add_plus;     // '+'
  bool add_space;    // ' '
  bool zero_pad;     // '0'
  bool alt_form;     // '#'
  bool precision;    // '.'
};

struct printf_parameters {
  size_t width;
  size_t precision;
  uint8_t base;
  struct printf_flags flags;
};

#endif
