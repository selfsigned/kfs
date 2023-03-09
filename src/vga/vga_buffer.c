#include "vga_internal.h"

uint16_t *vga_set_buffer_addr = (uint16_t *)VGA_HW_BUFFER_ADDR;
vga_global_info vga_state = {};

// private

/// @brief initialize the vga driver
void vga_buffer_init() {
  static bool init = false;

  // initalizing screens in RAM
  if (!init || (vga_state.hw_buffer_addr != vga_set_buffer_addr)) {
    vga_state.hw_vga_addr = (uint16_t *)VGA_HW_ADDR;
    vga_state.hw_buffer_addr = vga_set_buffer_addr;

    // populate addresses and clean them
    for (unsigned int i = 0; i < VGA_SCREEN_MAX; ++i) {
      vga_state.screen[i] = (vga_screen_info){
          .buffer = (vga_char *)((vga_state.hw_buffer_addr +
                                  (i * (VGA_SCREEN_MAX * sizeof(vga_char)))))};
      bzero(vga_state.screen[i].buffer, VGA_SCREEN_MAX * sizeof(vga_char));
    }
    init = true;
  }
}

// TODO scrolling cursor and history
int vga_buffer_writechar(vga_info *info, const unsigned char c) {
  int result = 0;
  unsigned char wrap_char = VGA_WRAP_DEFAULT_CHAR;
  vga_char vgac;           // character to write to screen
  uint16_t position;       // position on screen
  vga_screen_info *screen; // chosen screen

  vga_buffer_init();
  // WIP

  // load the currently set attributes
  screen = &vga_state.screen[info->screen];
  vgac = (vga_char){.c = c, .color = screen->attributes};

  // Wrap to next line
  if (info->column >= VGA_COL) {
    if (info->nowrap)
      return 0;
    if (info->wrapchar)
      wrap_char = info->wrapchar;
    // TODO row > 25 scroll mode
    info->column = 0;
    info->row++;
    result++;
    vga_buffer_writechar(info, wrap_char);
  }

  position = (info->row * VGA_COL) + info->column;
  screen->buffer[position] = vgac;

  info->column++;
  screen->last_info = *info;

  return ++result;
}

int vga_buffer_write(vga_info *info, const unsigned char *s) {
  const unsigned char *str = s;
  size_t result = 0;

  while (*str) {
    result += vga_buffer_writechar(info, *str);
    str++;
  }
  return result;
}

// public

int vga_showscreen(uint8_t screen_nbr) {
  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;

  memcpy(vga_state.hw_vga_addr, vga_state.screen[screen_nbr].buffer,
         VGA_SCREEN_SIZE * sizeof(vga_char));
  return 1;
}

void vga_clearscreen(uint8_t screen_nbr) {
  bzero(vga_state.screen[screen_nbr].buffer,
        VGA_SCREEN_SIZE * sizeof(vga_char));
}
