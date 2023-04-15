#ifndef LAYOUT_H
#define LAYOUT_H
// layout.h: Mode 1 keyboard mappings, provide APIs to change the current layout

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


extern const struct kbd_entry layout_us_mode1[KBD_KEYS_END];
extern const struct kbd_entry layout_fr_mode1[KBD_KEYS_END];
extern const struct kbd_entry layout_colemak_mode1[KBD_KEYS_END];

struct kbd_layout_info {
  const struct kbd_entry *layout;
  char layout_name[10];
};

/// default keyboard layout, set to qwerty
#define KBD_DEFAULT_LAYOUT layout_us_mode1

/// @brief list of available keymaps, null terminated
extern const struct kbd_layout_info kbd_available_layouts[];

/// @brief set this to an entry from `kbd_available_layouts`
extern kbd_entry const *kbd_layout;

#endif
