#ifndef DEBUG_H
#define DEBUG_H
// Debug facility macros

#include "drivers/vga/vga.h"

#define LOG_SCREEN 9

/// Print an error message to LOG_SCREEN, only insert the error msg and
/// parameters in format
#define ERROR_MSG(format, ...)                                                 \
  vga_printf((vga_info){.screen = LOG_SCREEN}, "ERROR: " format "\n",          \
             __VA_ARGS__);

/// Print a debug message to LOG_SCREEN, only insert the error msg and
/// parameters in format
#define DEBUG_MSG(format, ...)                                                 \
  vga_printf((vga_info){.screen = LOG_SCREEN}, "DEBUG: " format "\n",          \
             __VA_ARGS__);

#endif
