#ifndef DEBUG_H
#define DEBUG_H
// Debug facility macros

#include "drivers/vga/vga.h"
#include "kernel.h"

// #define DEBUG_ALL // to enable all the following
// #define DEBUG // if you want to enable debugging
// #define DEBUG_KBD // if you want to debug keycodes (spamy, slow)

/// Enable ALL debugging
#ifdef DEBUG_ALL
#define DEBUG
#define DEBUG_KBD
#endif

/// screen to write kernel messages to
#ifndef LOG_SCREEN
#error "LOG_SCREEN should be set in kernel.h!"
#endif

#define GENERIC_MSG(MSGLVL, format, ...)                                       \
  vga_printf((vga_info){.screen = LOG_SCREEN},                                 \
             #MSGLVL ": " format "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef DEBUG
/// Print a debug message to LOG_SCREEN in the fmt "DEBUG: format{parameters}"
#define DEBUG_MSG(format, ...)                                                 \
  GENERIC_MSG(DEBG, format __VA_OPT__(, ) __VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) (void)42;
#endif

/// Print an error message to LOG_SCREEN in the fmt "WARN: format{parameters}"
#define WARN_MSG(format, ...)                                                  \
  GENERIC_MSG(WARN, format __VA_OPT__(, ) __VA_ARGS__)

/// Print an info message to LOG_SCREEN in the fmt "INFO: format{parameters}"
#define INFO_MSG(format, ...)                                                  \
  GENERIC_MSG(INFO, format __VA_OPT__(, ) __VA_ARGS__)

#endif
