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

/// @brief VGA text character
typedef struct __attribute__((packed)) vga_char {
  unsigned char c : 8; /// the character we want to print, goes beyon ASCII
                       /// (TODO find exact charset)
  enum vga_color fg_color : 4; /// character color
  enum vga_color bg_color : 3; /// background color (goes only up to 8)
  bool blink : 1;              /// blink (INOP?)
} vga_char;

/// @brief control for vga print functions, modes are exclusive, default is
/// print at row, col
typedef struct vga_info {
  uint8_t screen; /// active screen buffer, negative value to use the last
  uint8_t row;    /// max 24 (starts at 0)
  uint8_t column; /// max 79 (stats at 0)
  bool noprint;   /// don't flush the screen buffer to VGA
  bool nocursor;  // TODO define
  bool scroll;    // TODO define
  bool nowrap;    /// wrap around line if output is too big
  char wrapchar;  /// the character to put when wrapping
} vga_info;

/// @brief output formatted string to a vga screen buffer (not posix)
/// @param info set the control info, pass a vga_info struct
/// @param format printf-like format string TODO document supported ops
/// @param ... arguments for printf format strings
/// @return negative if error, number of chars written otherwise
int vga_printf(vga_info info, const char *format, ...);

#endif
