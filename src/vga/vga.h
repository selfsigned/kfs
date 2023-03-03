#ifndef VGA_H
#define VGA_H

// VGA text driver

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

/// @brief A VGA text character
typedef struct __attribute__((packed)) {
  unsigned char c : 8; /// the character we want to print, goes beyon ASCII
                       /// (TODO find exact charset)
  enum vga_color fg_color : 4; /// character color
  enum vga_color bg_color : 3; /// background color (goes only up to 8)
  bool blink : 1;              /// blink
} vga_char;

#endif
