#include "../cp437.h"
#include "../drivers/vga/vga.h"

bool screen_create_box_light(uint8_t screen_nbr, uint8_t width,
                             uint8_t height) {
  uint8_t cursor_column;
  uint8_t cursor_row;

  if (!vga_screen_getcursorpos(screen_nbr, &cursor_column, &cursor_row) ||
      (cursor_column + width) >= VGA_COL || (cursor_row + height >= VGA_ROW))
    return false;

  // Vertical
  for (uint8_t i = cursor_row; i <= height + cursor_row; ++i) {
    uint8_t current_char = CP437_BOX_DRAWINGS_LIGHT_VERTICAL;
    vga_printf((vga_info){.screen = screen_nbr,
                          .nocursor = true,
                          .row = i,
                          .column = cursor_column},
               "%C", current_char);
    vga_printf((vga_info){.screen = screen_nbr,
                          .nocursor = true,
                          .row = i,
                          .column = cursor_column + width},
               "%C", current_char);
  }

  // Horizontal
  for (uint8_t i = cursor_column; i <= width + cursor_column; ++i) {
    uint8_t current_char = CP437_BOX_DRAWINGS_LIGHT_HORIZONTAL;

    if (i == cursor_column)
      current_char = CP437_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT;
    if (i == width)
      current_char = CP437_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT;
    vga_printf((vga_info){.screen = screen_nbr,
                          .nocursor = true,
                          .row = cursor_row,
                          .column = i},
               "%C", current_char);
    if (i == cursor_column)
      current_char = CP437_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT;
    if (i == width)
      current_char = CP437_BOX_DRAWINGS_LIGHT_UP_AND_LEFT;
    vga_printf((vga_info){.screen = screen_nbr,
                          .nocursor = true,
                          .row = cursor_row + height,
                          .column = i},
               "%C", current_char);
  }

  return true;
}
