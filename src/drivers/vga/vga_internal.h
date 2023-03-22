#ifndef VGA_INTERNAL_H
#define VGA_INTERNAL_H

#include "../../klibc/libc.h"
#include "vga.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// STATE //

/// @brief per screen vga state
typedef struct vga_screen_info {
  struct {
    uint8_t column;          /// store last column
    uint8_t row;             /// store last row
    bool vga;                /// show the vga cursor
  } cursor;                  /// store last position
  vga_attributes attributes; /// used to store current color attributes
  struct {
    vga_char *head; /// start of buffer in RAM
    vga_char *pos;  /// current position in buffer
    vga_char *tail; /// end of buffer
  } buffer;
  // vga_char *buffer; /// buffer address in RAM
} vga_screen_info;

/// @brief global vga state
typedef struct vga_global_info {
  vga_screen_info screen[VGA_SCREEN_MAX]; /// info per screen
  uint16_t *vga_addr;                     /// addr of the vga bios interface
  struct {
    uint16_t *addr;      /// addr of the buffer ram
    size_t history_size; /// amount of screens worth of history in buffer
    uint32_t size;       /// nbr of words (vga_char) in buffer !! multiply by
                         /// sizeof(vga_char) to cast to byte !!
  } buffer;              /// topology of a buffer
} vga_global_info;

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
/// @param is_ascii is the string ascii or CP437, used for special chars
/// @return numbers of char written or negative if error
int vga_buffer_write(vga_info *info, const unsigned char *s, bool is_ascii);

#endif
