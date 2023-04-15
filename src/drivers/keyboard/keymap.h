#ifndef KEYMAP_H
#define KEYMAP_H
// keymap.h: Mode 1 keyboard mappings

#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>

enum KBD_LAYOUT_FLAGS { FLAG_CAPSLOCK = 0x0, FLAG_TOGGLE = 0x2 };

/// MODE1 keyboard layout mapping
typedef struct kbd_entry {
  uint8_t flags;
  kbd_scancode code;
  char normal;
  char shift;
  char alt;
} kbd_entry;

/// US mode 1 layout
extern const struct kbd_entry layout_us_mode1[KBD_KEYS_END];

#endif
