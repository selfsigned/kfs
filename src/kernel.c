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

  vga_printf((vga_info){}, "This should be invisible2\n");
  vga_printf((vga_info){}, "This should be invisible\n");
  vga_printf((vga_info){.row = 24}, "\n\n"); // put stuff above in history
  vga_printf((vga_info){.screen = 0, .row = 0, .column = 0, .setcursor = true},
             "X%c%C%c", '4', CP437_HEART, '2');
  vga_printf((vga_info){.screen = 0, .row = 3, .column = 70, .nowrap = false},
             "wrapping and %%s are: %s",
             "and so is \\n \n in parameter strings");
  vga_printf((vga_info){.screen = 0, .row = 24, .column = 78}, "%CY",
             CP437_YEN_SIGN);
  vga_printf((vga_info){.screen = 0, .row = 12, .column = 10, .nocursor = true},
             "int: %i hex: %x unsigned:%u octal: %o binary: %b", -42, 42, 42,
             42, 42);
  // vga_screen_clear(0);
  vga_screen_show_scrolled(0, 0, true);

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;
  kernel_wait();
}
