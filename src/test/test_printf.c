#include "../cp437.h"
#include "../drivers/vga/vga.h"
#include "test.h"

void test_printf(uint8_t screen_nbr) {

  // test background
  vga_screen_fillbackground(screen_nbr, VGA_COLOR_BLUE);

  // test scroll
  vga_printf((vga_info){.screen = screen_nbr}, "This shouldn't be in view\n");
  vga_printf((vga_info){.screen = screen_nbr},
             "This REALLY shouldn't be in view\n");
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
      "int: %i hex:%x hexUP:%#X unsigned:%u octal: %#o binary: %#b", -42, 42,
      42, 42, 42, 42);
  vga_printf((vga_info){.screen = screen_nbr, .row = 24, .column = 78}, "%CY",
             CP437_YEN_SIGN);
}

void test_vga_cp437(uint8_t screen_nbr) {
  uint8_t cp437[255] = {0};

  for (uint8_t c = 1; c < 255; ++c)
    cp437[c] = c;
  vga_printf((vga_info){.screen = screen_nbr, .nowrapchar = true}, "%C%S", 0,
             cp437 + 1);
}

void test_vga_color(uint8_t screen_nbr, uint8_t test_char) {
  for (uint8_t bg = 0; bg < 8; ++bg) {
    for (uint8_t fg = 0; fg < 16; ++fg) {
      vga_printf((vga_info){.screen = screen_nbr, .nowrap = true}, "%a%C",
                 (vga_attributes){.bg = bg, .fg = fg}, test_char);
    }
    vga_printf((vga_info){.screen = screen_nbr}, "\n");
  }
}
