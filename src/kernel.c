#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "klibc/libc.h"
#include "vga/vga.h"

#include "cp437.h"

/// @brief Wait indefinitely
void __attribute__((noinline)) kernel_wait(void) {
  while (42) {
  }
}

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  int *memory_signature = (void *)SIGNATURE_ADDRESS;

  vga_init(16, (uint16_t *)SCREEN_BUFFER_ADDR);
  // using screen 0 by default
  // vga_screen_fillattributes(0, (vga_attributes){.bg = VGA_COLOR_BLUE});
  vga_screen_fillbackground(0, VGA_COLOR_BLUE);
  vga_printf((vga_info){}, "This should be invisible2\n");
  vga_printf((vga_info){}, "This should be invisible\n");
  vga_printf((vga_info){.row = 24, .scrollattributes = true},
             "\n\n"); // put stuff above in history
  vga_printf((vga_info){.screen = 0, .row = 0, .column = 0, .setcursor = true},
             "%aX%c%C%c",
             (vga_attributes){.bg = VGA_COLOR_CYAN, .fg = VGA_COLOR_WHITE}, '4',
             CP437_HEART, '2');
  vga_printf((vga_info){.screen = 0, .row = 3, .column = 70, .nowrap = false},
             "wrapping and %%s are: %s",
             "implemented and so is \\n \n in parameter strings");
  vga_printf((vga_info){.screen = 0, .row = 12, .column = 10, .nocursor = true},
             "int: %i hex: %x unsigned:%u octal: %o binary: %b", -42, 42, 42,
             42, 42);
  vga_printf((vga_info){.screen = 0, .row = 24, .column = 78}, "%CY",
             CP437_YEN_SIGN);
  vga_screen_show_scrolled(0, 0, true);

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;
  kernel_wait();
}
