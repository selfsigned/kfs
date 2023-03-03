#ifndef VGA_INTERNAL_H
#define VGA_INTERNAL_H

#include "../klibc/libc.h"
#include "vga.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// HW address of the VGA buffer
#define VGA_HW_ADDR 0xB8000

#define VGA_ROW 25
#define VGA_COL 80
/// VGA_ROW * VGA_COL
#define VGA_SCREEN_SIZE 2000

/// @brief per screen vga state
typedef struct vga_screen_info {
  vga_info last_info; // store last info
  vga_char last_char; // used to store last char info
  vga_char **buffer;  // buffer address in VRAM
} vga_screen_info;

/// @brief global vga state
typedef struct vga_global_info {
  vga_screen_info screen[VGA_SCREEN_MAX]; // info per screen
  uint8_t current_screen;                 // current screen
  unsigned char screen_max;               // number of screens
  uint16_t *hw_addr;                      // addr of the vga bios interface
  uint16_t *hw_buffer_addr;               // addr of the buffer ram
} vga_global_info;

// TODO
int vga_buffer_write(vga_info info, vga_char c);

#endif
