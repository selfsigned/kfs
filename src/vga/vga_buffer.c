#include "vga_internal.h"

uint16_t *vga_set_buffer_addr = (uint16_t *)VGA_HW_BUFFER_ADDR;
vga_global_info vga_state = {};

/// @brief initialize the vga driver
void vga_buffer_init() {
  static bool init = false;

  // initalizing screens in RAM
  if (!init || (vga_state.hw_buffer_addr != vga_set_buffer_addr)) {
    vga_state.screen_max = VGA_SCREEN_MAX;
    vga_state.hw_vga_addr = (uint16_t *)VGA_HW_ADDR;
    vga_state.hw_buffer_addr = vga_set_buffer_addr;

    // populate addresses and clean them
    for (unsigned int i = 0; i < vga_state.screen_max; ++i) {
      vga_state.screen[i] = (vga_screen_info){
          .buffer =
              (vga_char *)((vga_state.hw_buffer_addr + (i * VGA_SCREEN_SIZE)))};
      bzero(vga_state.screen[i].buffer, VGA_SCREEN_SIZE);
    }
    init = true;
  }
}

void vga_buffer_flush(vga_info info) {
  memcpy(vga_state.hw_vga_addr, vga_state.screen[info.screen].buffer,
         sizeof(vga_char) * VGA_SCREEN_SIZE);
}

int vga_buffer_write(vga_info *info, vga_char c) {
  uint16_t position;
  vga_screen_info *screen;

  vga_buffer_init();
  // WIP

  // Wrap to next line
  if (info->column > 79) {
    if (info->nowrap)
      return 0;
    vga_char wrap_char;
    // TODO row > 25
    info->column = 0;
    info->row++;

    wrap_char = c;
    if (!info->wrapchar)
      info->wrapchar = VGA_WRAP_DEFAULT_CHAR;
    wrap_char.c = info->wrapchar;
    vga_buffer_write(info, wrap_char);
  }

  position = (info->row * 80) + info->column;
  screen = &vga_state.screen[info->screen];
  screen->buffer[position] = c;

  info->column++;

  return 1;
}
