#ifndef PRINTF_INTERNAL_H
#define PRINTF_INTERNAL_H

#include <stdbool.h>

struct printf_flags {
  bool left_justify; // '-'
  bool add_plus;     // '+'
  bool add_space;    // ' '
  bool zero_pad;     // '0'
  bool alt_form;     // '#'
  bool precision;    // '.'
};

struct printf_parameters {
  int width;
  int precision;
  struct printf_flags flags;
};

#endif
