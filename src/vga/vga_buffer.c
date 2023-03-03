#include "vga_internal.h"

uint16_t *vga_set_buffer_addr = (uint16_t *)VGA_HW_BUFFER_ADDR;
vga_global_info vga_state = {};

void vga_buffer_init() {
  static bool init = false;

  // initalizing screens in RAM
  if (!init || (vga_state.hw_buffer_addr != vga_set_buffer_addr)) {
    vga_state.screen_max = VGA_SCREEN_MAX;
    vga_state.hw_addr = (uint16_t *)VGA_HW_ADDR;
    vga_state.hw_buffer_addr = vga_set_buffer_addr;

    // populate addresses and clean them
    for (unsigned int i = 0; i < vga_state.screen_max; ++i) {
      vga_state.screen[i] =
          (vga_screen_info){.buffer = (vga_char **)((vga_state.hw_buffer_addr +
                                                     (i * VGA_SCREEN_SIZE)))};
      bzero(vga_state.screen[i].buffer, VGA_SCREEN_SIZE);
    }
    init = true;
  }
}

int vga_buffer_write(vga_info info, vga_char c) {
  vga_screen_info screen;

  vga_buffer_init();
  // TODO this is just a poc
  screen = vga_state.screen[info.screen];
  *screen.buffer[info.column] = c;
  if (!info.noprint) {
    memcpy(*vga_state.hw_addr, *screen.buffer, VGA_SCREEN_SIZE);
  }
  // vga_state.hw_addr = 'c';
  return 0;
};
