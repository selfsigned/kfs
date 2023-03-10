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
  struct {
    uint8_t column;          /// store last column
    uint8_t row;             /// store last row
  } cursor;                  /// store last position
  vga_attributes attributes; /// used to store current color attributes
  vga_char *buffer;          /// buffer address in VRAM
} vga_screen_info;

/// @brief global vga state
typedef struct vga_global_info {
  vga_screen_info screen[VGA_SCREEN_MAX]; // info per screen
  uint16_t *hw_vga_addr;                  // addr of the vga bios interface
  uint16_t *hw_buffer_addr;               // addr of the buffer ram
} vga_global_info;

// DRIVER //

#define VGA_WRAP_DEFAULT_CHAR '>'

// BUFFER //

/// @brief load the cursor position depending on the info parameters, optinally
/// return to line
/// @param info the parameters to edit the cursor with
/// @param insert_newline add a newline for the cursor
/// @return the initalized screen
vga_screen_info *vga_set_cursor(vga_info *info, bool insert_newline);

/// @brief write char to a buffer
/// @param info buffer write control struct
/// @param c char to write to buffer
/// @return numbers of char written or negative value if error
int vga_buffer_writechar(vga_info *info, const unsigned char c);

/// @brief write string to buffer
/// @param info buffer write control struct
/// @param s string to write to buffer
/// @return numbers of char written or negative if error
int vga_buffer_write(vga_info *info, const unsigned char *s);

#endif
