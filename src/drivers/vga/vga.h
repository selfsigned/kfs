#ifndef VGA_H
#define VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// VGA text driver //

/// max number of screens
#define VGA_SCREEN_MAX 12

/// default character used when wrapping
#define VGA_WRAP_DEFAULT_CHAR '>'

/// HW address of the VGA buffer
#define VGA_HW_ADDR 0xB8000

#define VGA_ROW 25
#define VGA_COL 80
/// VGA_ROW * VGA_COL
#define VGA_SCREEN_SIZE 2000

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
  uint8_t column;         /// start at, max 79 (starts at 0)
  uint8_t row;            /// start at, max 24 (starts at 0)
  bool setcursor;         /// sets the cursor to column and row
                          /// neededed if row == 0 || col == 0
  bool nocursor;          /// append without loading or change the cursor state
  bool nowrap;            /// stop printing if the column overflows
  bool nowrapchar;        /// avoid printing a wrap character
  bool noscroll;          /// stop printing if the row overflows
  bool scrollattributes;  /// carry attributes over when scrolling
  bool noattributes;      /// don't load color info from the screen state
  bool print;             /// flush the screen buffer to VGA
  unsigned char wrapchar; /// the character to put when wrapping
  struct {
    bool cursor_loaded; /// !INTERNAL! is the screen cursor loaded?
  } internal; /// DO NOT TOUCH, sad sad sad lack of separation of concern
} vga_info;

// Functions //

// driver init

/// @brief initializes the vga driver, call before any other vga function
/// @param history_size needs to be at least 1, amount of screens worth of
/// history, N * 80*25 * sizeof(vga_char).
/// @param buffer_addr address of the screen buffers in memory
/// @return false if the screen doesn't exist or is already initalized
bool vga_init(size_t history_size, uint16_t *buffer_addr);

/// @brief output formatted string to a vga screen buffer (not posix)
/// @param info set the control info, pass a vga_info struct
/// @param format printf-like format string TODO document supported ops
/// @param ... arguments for printf format strings
/// @return negative if error, number of chars written otherwise
int vga_printf(vga_info info, const char *format, ...);

// getters/setters

/// @brief get the position of the cursor into column and row
/// @param screen_nbr screen to get the cursor position of
/// @param column destination variable for the column position
/// @param row destionation variable for the row position
/// @return negative if screen doesn't exist
bool vga_screen_getcursorpos(uint8_t screen_nbr, uint8_t *column, uint8_t *row);

/// @brief set the position of the cursor
/// @param screen_nbr screen to set the cursor position in
/// @param column cursor column position (starts at 0)
/// @param row cursor row position (starts at 0)
/// @return negative if screen doesn't exist or values overflow
bool vga_screen_setcursorpos(uint8_t screen_nbr, uint8_t column, uint8_t row);

/// @brief set color attributes for next characters on a given screen
/// @param screen_nbr nbr of the screen to change attrs on
/// @param attributes color attributes to set
/// @return false if screen doesn't exist
bool vga_screen_setattributes(uint8_t screen_nbr, vga_attributes attributes);

/// @brief replace character color attributes for a given screen and set them
/// @param screen_nbr nbr of the screen to edit
/// @param attributes color attributes to override existing values with
/// @return false if screen doesn't exist
bool vga_screen_fillattributes(uint8_t screen_nbr, vga_attributes attributes);

/// @brief replace background color for a given screen and set it
/// @param screen_nbr nbr of the screen to edit
/// @param background_color background color to fill the screen with
/// @return false if screen doesn't exist
bool vga_screen_fillbackground(uint8_t screen_nbr,
                               enum vga_color background_color);

// display

/// @brief display the selected screen buffer
/// @param screen_nbr screen to print
/// @return false if screen doesn't exist
bool vga_screen_show(uint8_t screen_nbr);

/// @brief display the selected screen buffer, scrolled
/// @param screen_nbr screen to print
/// @param rows number of rows to scroll up by, -1 to go at the top
/// TODO @param scroll_overlay display position in buffer
/// @return negative if screen doesn't exist, 0 if at buffer head
int vga_screen_show_scrolled(uint8_t screen_nbr, int rows);

// reset

/// @brief clear the screen and reset cursor info
/// @param screen_nbr screen to clear
/// @return false if screen doesn't exist
bool vga_screen_clear(uint8_t screen_nbr);

#endif
