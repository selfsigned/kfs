#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../log.h"
#include <stdbool.h>
#include <stdint.h>

/// set this to enable keyboard key debugging (spam-y)
#ifdef DEBUG_KBD
#define DEBUG_KBD_MSG(format, ...)                                             \
  GENERIC_MSG(DEBUG_KBD, format __VA_OPT__(, ) __VA_ARGS__)
#else
#define DEBUG_KBD_MSG(format, ...) (void)42;
#endif

/// the bit at this address is set when the key is released
#define KBD_RELEASED_OFFSET 0x80
#define KBD_EXTENDED_KEYS 0xE0

// scancode set 1
// https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1
typedef enum kbd_scancode {
  KBD_UNDEFINED = 0,

  KBD_ESCAPE = 0x1, // Row 1
  KBD_1,
  KBD_2,
  KBD_3,
  KBD_4,
  KBD_5,
  KBD_6,
  KBD_7,
  KBD_8,
  KBD_9,
  KBD_0,
  KBD_DASH,
  KBD_EQUAL,
  KBD_BACKSPACE,

  KBD_TAB = 0x0f, // Row 2
  KBD_Q,
  KBD_W,
  KBD_E,
  KBD_R,
  KBD_T,
  KBD_Y,
  KBD_U,
  KBD_I,
  KBD_O,
  KBD_P,
  KBD_OPENING_BRACKET,
  KBD_CLOSING_BRACKET,
  KBD_ENTER,

  KBD_LEFT_CTRL = 0x1D, // Row 3
  KBD_A,
  KBD_S,
  KBD_D,
  KBD_F,
  KBD_G,
  KBD_H,
  KBD_J,
  KBD_K,
  KBD_L,
  KBD_SEMICOLON,
  KBD_SINGLE_QUOTE,
  KBD_BACKTICK,

  KBD_LEFT_SHIFT = 0x2A, // Row 4
  KBD_BACKSLASH,
  KBD_Z,
  KBD_X,
  KBD_C,
  KBD_V,
  KBD_B,
  KBD_N,
  KBD_M,
  KBD_COMMA,
  KBD_DOT,
  KBD_SLASH,
  KBD_RIGHT_SHIFT,

  KBD_KEYPAD_ASTERISK = 0x37,

  KBD_LEFT_ALT = 0x38, // Row 5
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
  KBD_KEYPAD_7,
  KBD_KEYPAD_8,
  KBD_KEYPAD_9,
  KBD_KEYPAD_DASH,
  KBD_KEYPAD_4,
  KBD_KEYPAD_5,
  KBD_KEYPAD_6,
  KBD_KEYPAD_PLUS,
  KBD_KEYPAD_1,
  KBD_KEYPAD_2,
  KBD_KEYPAD_3,
  KBD_KEYPAD_0,
  KBD_KEYPAD_DOT,
  KBD_F11,
  KBD_F12,

  // 0x81 [...] 0xD8 same but released (& 0x80)

  // extended keys set with 0xE0
  KBD_MULTIMEDIA_PREVIOUS_TRACK = 0x10 + KBD_EXTENDED_KEYS,
  KBD_MULTIMEDIA_NEXT_TRACK = 0x19 + KBD_EXTENDED_KEYS,

  KBD_KEYPAD_ENTER = 0x1C + KBD_EXTENDED_KEYS,
  KBD_RIGHT_CTRL,

  KBD_MULTIMEDIA_MUTE = 0x20 + KBD_EXTENDED_KEYS,
  KBD_MULTIMEDIA_CALCULATOR,
  KBD_MULTIMEDIA_PLAY,
  KBD_MULTIMEDIA_STOP = 0x24 + KBD_EXTENDED_KEYS,

  KBD_MULTIMEDIA_VOLUME_DOWN = 0x2E + KBD_EXTENDED_KEYS,
  KBD_MULTIMEDIA_VOLUME_UP = 0x30 + KBD_EXTENDED_KEYS,

  KBD_MULTIMEDIA_HOME_WWW = 0x32 + KBD_EXTENDED_KEYS,

  KBD_KEYPAD_SLASH = 0x35 + KBD_EXTENDED_KEYS,
  KBD_RIGHT_ALT = 0x38 + KBD_EXTENDED_KEYS,

  KBD_HOME = 0x47 + KBD_EXTENDED_KEYS,
  KBD_CURSOR_UP,
  KBD_PAGE_UP,
  KBD_CURSOR_LEFT = 0x4B + KBD_EXTENDED_KEYS,
  KBD_CURSOR_RIGHT = 0x4D + KBD_EXTENDED_KEYS,
  KBD_END = 0x4F + KBD_EXTENDED_KEYS,
  KBD_CURSOR_DOWN = 0x50 + KBD_EXTENDED_KEYS,
  KBD_PAGE_DOWN,
  KBD_INSERT,
  KBD_DELETE,

  KBD_LEFT_GUI = 0x5B + KBD_EXTENDED_KEYS,
  KBD_RIGHT_GUI,
  KBD_APPS,
  KBD_ACPI_POWER,
  KBD_ACPI_SLEEP,
  KBD_ACPI_WAKE = 0x63 + KBD_EXTENDED_KEYS,

  KBD_MULTIMEDIA_WWW_SEARCH = 0x65 + KBD_EXTENDED_KEYS,
  KBD_MULTIMEDIA_WWW_FAVORITES,
  KBD_MULTIMEDIA_WWW_REFRESH,
  KBD_MULTIMEDIA_WWW_STOP,
  KBD_MULTIMEDIA_WWW_FORWARD,
  KBD_MULTIMEDIA_WWW_BACK,
  KBD_MULTIMEDIA_MY_COMPUTER,
  KBD_MULTIMEDIA_EMAIL,
  KBD_MULTIMEDIA_MEDIA_SELECT,

  // used in key press/release detection
  KBD_EXTENDED_KEYS_END = KBD_MULTIMEDIA_MEDIA_SELECT,

  // needed to set the right size
  KBD_KEYS_END = KBD_EXTENDED_KEYS_END + KBD_EXTENDED_KEYS,
} kbd_scancode;

/// @brief modifiers currently enabled
struct kbd_modifiers {
  bool capslock : 1;
  bool numlock : 1;
  bool scroll_lock : 1;
  bool shift : 1;
  bool ctrl : 1;
  bool alt : 1;
};

/// @brief current keypress state
enum kbd_event {
  KEY_RELEASED = 0,
  KEY_PRESSED,
};

/// @brief keep track of the keyboard state
struct kbd_state {
  struct kbd_modifiers mod; /// active modifiers
  enum kbd_event status;    /// is the key pressed or released
  kbd_scancode key;         /// current key
  char ascii;               /// current key converted to ascii
};

/// @brief Initialize the keyboard driver !INITIALIZE INTERRUPTS FIRST!
void kbd_init();

/// @brief Interface to read the current keyboard state
extern struct kbd_state kbd;

/// @brief controls the keyboard scroll lock indicator led
/// @param enabled enable or disable the led
void kbd_led_scroll(bool enabled);
/// @brief controls the keyboard numeric lock indicator led
/// @param enabled enable or disable the led
void kbd_led_num(bool enabled);
/// @brief controls the keyboard caps lock indicator led
/// @param enabled enable or disable the led
void kbd_led_caps(bool enabled);

/// @brief Poll the keyboard,
/// @warning Don't use this if you don't have to, an interrupt interface is
/// already provided and this doesn't support localization
/// @return kbd_scancode read scancode (in QWERTY)
kbd_scancode kbd_poll();

#endif
