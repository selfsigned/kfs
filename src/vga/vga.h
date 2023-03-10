#ifndef VGA_H
#define VGA_H

#include <stdbool.h>
#include <stdint.h>

// VGA text driver

/// set this to change the address of the buffers in memory
extern uint16_t *vga_set_buffer_addr;
// default HW address for vga_set_buffer_addr;
#define VGA_HW_BUFFER_ADDR 0x001FFFFF

/// max number of screens
#define VGA_SCREEN_MAX 9

/// @brief VGA colors, goes only up to 8 for the background
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

typedef struct __attribute__((packed)) vga_attributes {
  enum vga_color fg : 4; /// character color
  enum vga_color bg : 3; /// background color (goes only up to 8)
  bool blink : 1;        /// blink (INOP?)
} vga_attributes;

/// @brief VGA text character
typedef struct __attribute__((packed)) vga_char {
  unsigned char c : 8;         /// CP437 character to print
  struct vga_attributes color; /// character color attributes
} vga_char;

/// @brief print control for vga print function.
/// Just partially initialize with what you actually need
typedef struct vga_info {
  uint8_t screen;         /// screen buffer to print to
  uint8_t column;         /// start at, max 79 (stats at 0)
  uint8_t row;            /// start at, max 24 (starts at 0)
  bool setcursor;         /// sets the cursor to column and row
                          /// neededed if row == 0 || col == 0
  bool nocursor;          /// append without loading or change the cursor state
  bool nowrap;            /// stop printing if the column overflows
  bool noscroll;          /// stop printing if the row overflows
  bool noattributes;      /// don't load color info from the screen state
  bool print;             /// flush the screen buffer to VGA
  unsigned char wrapchar; /// the character to put when wrapping
  struct {
    bool cursor_loaded; /// !INTERNAL! is the screen cursor loaded?
  } internal; /// DO NOT TOUCH, sad sad sad lack of separation of concern
} vga_info;

/// @brief output formatted string to a vga screen buffer (not posix)
/// @param info set the control info, pass a vga_info struct
/// @param format printf-like format string TODO document supported ops
/// @param ... arguments for printf format strings
/// @return negative if error, number of chars written otherwise
int vga_printf(vga_info info, const char *format, ...);

/// @brief set color attributes for a given screen
/// @param screen_nbr
/// @param attributes
/// @return negative if screen doesn't exist
int vga_screen_setattributes(uint8_t screen_nbr, vga_attributes attributes);

/// @brief flush the selected screen buffer to vga, print as well
/// @param screen_nbr screen to print
/// @return negative if screen doesn't exist
int vga_screen_show(uint8_t screen_nbr);

/// @brief clear the screen and reset cursor info
/// @param screen_nbr screen to clear
/// @return negative if screen doesn't exist
int vga_screen_clear(uint8_t screen_nbr);

#endif
