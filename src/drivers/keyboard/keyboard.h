#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

/// the bit at this address is set when the key is released
#define KBD_RELEASED_OFFSET 0x80

// scancode set 1
// https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1
typedef enum scancode {
  KBD_ESCAPE = 0x1,
  KBD_BACKSPACE = 0x0E,
  KBD_TAB,
  KBD_ENTER = 0x1C,
  KBD_LEFT_CTRL,
  KBD_LEFT_SHIFT = 0x2A,
  KBD_RIGHT_SHIFT = 0x36,
  KBD_LEFT_ALT = 0x38,
  KBD_SPACE,
  KBD_CAPSLOCK,
  KBD_F1,
  KBD_F2,
  KBD_F3,
  KBD_F4,
  KBD_F5,
  KBD_F6,
  KBD_F7,
  KBD_F8,
  KBD_F9,
  KBD_F10,
  KBD_NUMLOCK,
  KBD_SCROLLLOCK,
  KBD_F11 = 0x57,
  KBD_F12,
  // Hack from here, only released
  KBD_HOME = 0xC7,
  KBD_CURSOR_UP,
  KBD_PAGE_UP,
  KBD_CURSOR_LEFT = 0xCB,
  KBD_CURSOR_RIGHT = 0xCD,
  KBD_END = 0xCF,
  KBD_CURSOR_DOWN,
  KBD_PAGE_DOWN,
  KBD_INSERT,
  KBD_DELETE,
} scancode;

struct kbd_modifiers {
  bool capslock : 1;
  bool numlock : 1;
  bool scroll_lock : 1;
  bool shift : 1;
  bool ctrl : 1;
  bool alt : 1;
};

struct kbd_state {
  scancode key;             /// current key
  char ascii;               /// current key converted to ascii
  struct kbd_modifiers mod; /// active modifiers
};

// http://www.osdever.net/bkerndev/Docs/keyboard.htm
extern unsigned char us_scancode_1[128];

/// @brief Initialize the keyboard !!INIT INTERRUPTS FIRST!!
void kbd_init();

/// @brief Interface to read the current keyboard state
extern struct kbd_state kbd;

char kbd_code_to_ascii(scancode code);

/// @brief Poll the keyboard, Don't use this if you don't have to
/// @return scancode
scancode kbd_poll();

/// @brief controls the keyboard scroll lock indicator led
/// @param enabled enable or disable the led
void kbd_led_scroll(bool enabled);
/// @brief controls the keyboard numeric lock indicator led
/// @param enabled enable or disable the led
void kbd_led_num(bool enabled);
/// @brief controls the keyboard caps lock indicator led
/// @param enabled enable or disable the led
void kbd_led_caps(bool enabled);

#endif
