#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel.h"
#include "klibc/libc.h"
#include "vga/vga.h"

/// @brief Wait indefinitely
void __attribute__((noinline)) kernel_wait(void) {
  while (42) {
  }
}

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  int *memory_signature = (void *)SIGNATURE_ADDRESS;

  vga_printf((vga_info){.row = 0, .column = 0, .noprint = true}, "X");
  vga_printf((vga_info){.row = 9, .column = 39, .noprint = true}, "42");
  vga_printf(
      (vga_info){.row = 10, .column = 73, .noprint = true, .wrapchar = '|'},
      "wrapping is implemented");
  vga_printf((vga_info){.row = 24, .column = 79}, "X");

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;
  kernel_wait();
}
