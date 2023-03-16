#include "../cp437.h"
#include "../drivers/vga/vga.h"
#include "test.h"

void test_vga(uint8_t screen_nbr, bool scroll, bool print) {

  // test background
  vga_screen_fillbackground(screen_nbr, VGA_COLOR_BLUE);

  // test scroll
  vga_printf((vga_info){.screen = screen_nbr}, "This should be invisible2\n");
  vga_printf((vga_info){.screen = screen_nbr}, "This should be invisible\n");
  vga_printf(
      (vga_info){.screen = screen_nbr, .row = 24, .scrollattributes = true},
      "\n\n"); // put stuff above in history

  // test buffer upper and CP437 chars parameters
  vga_printf(
      (vga_info){
          .screen = screen_nbr, .row = 0, .column = 0, .setcursor = true},
      "%aX%c%C%c",
      (vga_attributes){.bg = VGA_COLOR_CYAN, .fg = VGA_COLOR_WHITE}, '4',
      CP437_HEART, '2');

  // test strings and escapes
  vga_printf(
      (vga_info){.screen = screen_nbr, .row = 3, .column = 70, .nowrap = false},
      "wrapping and %%s are: %s",
      "implemented and so is \\n \n in parameter strings");

  // test numbers
  vga_printf(
      (vga_info){
          .screen = screen_nbr, .row = 12, .column = 10, .nocursor = true},
      "int: %i hex: %x unsigned:%u octal: %o binary: %b", -42, 42, 42, 42, 42);
  vga_printf((vga_info){.screen = screen_nbr, .row = 24, .column = 78}, "%CY",
             CP437_YEN_SIGN);

  // print
  if (print)
    vga_screen_show_scrolled(screen_nbr, (scroll) ? 2 : 0);
}
