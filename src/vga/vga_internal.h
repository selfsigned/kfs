#ifndef VGA_INTERNAL_H
#define VGA_INTERNAL_H

#include "../klibc/libc.h"
#include "vga.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// HW //

/// HW address of the VGA buffer
#define VGA_HW_ADDR 0xB8000

#define VGA_ROW 25
#define VGA_COL 80
/// VGA_ROW * VGA_COL
#define VGA_SCREEN_SIZE 2000

// STATE //

/// @brief per screen vga state
typedef struct vga_screen_info {
  vga_info last_info; // store last info
  vga_char last_char; // used to store last char info
  vga_char *buffer;   // buffer address in VRAM
} vga_screen_info;

/// @brief global vga state
typedef struct vga_global_info {
  vga_screen_info screen[VGA_SCREEN_MAX]; // info per screen
  uint8_t current_screen;                 // current screen
  unsigned char screen_max;               // number of screens
  uint16_t *hw_vga_addr;                  // addr of the vga bios interface
  uint16_t *hw_buffer_addr;               // addr of the buffer ram
} vga_global_info;

// DRIVER //

#define VGA_WRAP_DEFAULT_CHAR '>'

// BUFFER //

/// @brief write char to a buffer
/// @param info buffer write control struct
/// @param c char to write to buffer
/// @return numbers of char written or negative value if error
int vga_buffer_write(vga_info *info, vga_char c);

/// @brief flush buffer to screen
/// @param info buffer write control struct, only screen is used here
void vga_buffer_flush(vga_info info);

#endif
