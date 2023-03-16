#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/vga/vga.h"
#include "kernel.h"
#include "klibc/libc.h"

#include "test/test.h"

/// @brief Wait indefinitely
void __attribute__((noinline)) kernel_wait(void) {
  while (42) {
  }
}

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  int *memory_signature = (void *)SIGNATURE_ADDRESS;

  // initialized the vga driver with an history size of 16*25 rows
  vga_init(16, (uint16_t *)SCREEN_BUFFER_ADDR);
  test_vga(9, false, true); // write tests on scr 9 w/o scrolling and print them

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;

  kernel_wait();
}
