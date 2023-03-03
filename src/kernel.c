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
  uint16_t *vga_text_buffer = (uint16_t *)0xB8000;

  // print dumb flag
  memcpy(&(vga_text_buffer[1000]),
         &((vga_char){.c = '4',
                      .fg_color = VGA_COLOR_WHITE,
                      .bg_color = VGA_COLOR_BLUE}),
         sizeof(vga_char));
  memcpy(&(vga_text_buffer[1001]),
         &((vga_char){.c = ' ', .bg_color = VGA_COLOR_LIGHT_GREY}),
         sizeof(vga_char));
  memcpy(&(vga_text_buffer[1002]),
         &((vga_char){
             .c = '2', .fg_color = VGA_COLOR_WHITE, .bg_color = VGA_COLOR_RED}),
         sizeof(vga_char));

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;
  kernel_wait();
}
