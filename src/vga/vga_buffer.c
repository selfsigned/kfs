#include "vga_internal.h"

uint16_t *vga_set_buffer_addr = (uint16_t *)VGA_HW_BUFFER_ADDR;
vga_global_info g_vga_state = {};

// private

/// @brief initialize the vga driver
void vga_buffer_init() {
  static bool init = false;

  // initalizing screens in RAM
  if (!init || (g_vga_state.hw_buffer_addr != vga_set_buffer_addr)) {
    g_vga_state.hw_vga_addr = (uint16_t *)VGA_HW_ADDR;
    g_vga_state.hw_buffer_addr = vga_set_buffer_addr;

    // populate addresses and clean them
    for (unsigned int i = 0; i < VGA_SCREEN_MAX; ++i) {
      g_vga_state.screen[i] = (vga_screen_info){
          .buffer = (vga_char *)((g_vga_state.hw_buffer_addr +
                                  (i * (VGA_SCREEN_MAX * sizeof(vga_char)))))};
      bzero(g_vga_state.screen[i].buffer, VGA_SCREEN_MAX * sizeof(vga_char));
    }
    init = true;
  }
}

vga_screen_info *vga_set_cursor(vga_info *info, bool insert_newline) {
  vga_screen_info *screen = &g_vga_state.screen[info->screen];

  // load cursor info from the screen state once
  if ((!info->nocursor || info->setcursor) && !info->internal.cursor_loaded) {
    // if you want to set that to 0 you'll need nocursor, that's an obvious flaw
    if (!info->column && !info->setcursor)
      info->column = screen->cursor.column;
    if (!info->row && !info->setcursor)
      info->row = screen->cursor.row;
    info->internal.cursor_loaded = true;
  }

  // TODO scroll
  if (insert_newline && info->row <= VGA_ROW) {
    info->column = 0;
    info->row++;
    if (!info->nocursor) {
      screen->cursor.column = 0;
      screen->cursor.row++;
    }
  }

  return screen;
}

// TODO scrolling cursor and history
int vga_buffer_writechar(vga_info *info, const unsigned char c) {
  int result = 0;
  unsigned char wrap_char = VGA_WRAP_DEFAULT_CHAR;
  vga_char vgac;           // character to write to screen
  uint16_t position;       // position on screen
  vga_screen_info *screen; // chosen screen

  vga_buffer_init();

  // Wrap to next line
  if (info->column >= VGA_COL) {
    // TODO scroll
    if (info->nowrap || info->row >= VGA_ROW)
      return 0;

    if (info->wrapchar)
      wrap_char = info->wrapchar;
    info->column = 0;
    info->row++;
    result++;
    vga_buffer_writechar(info, wrap_char);
  }

  // load attributes and cursor state
  screen = vga_set_cursor(info, false);
  vgac = (vga_char){.c = c,
                    .color = (!info->noattributes) ? screen->attributes
                                                   : (vga_attributes){}};

  // set character in buffer
  position = (info->row * VGA_COL) + info->column;
  screen->buffer[position] = vgac;

  // set cursor state
  info->column++;
  if (!info->nocursor || info->setcursor) {
    screen->cursor.column = info->column;
    screen->cursor.row = info->row;
  }

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

int vga_screen_setattributes(uint8_t screen_nbr, vga_attributes attributes) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;

  screen = &g_vga_state.screen[screen_nbr];
  screen->attributes = attributes;

  return 1;
}

int vga_screen_show(uint8_t screen_nbr) {
  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;

  memcpy(g_vga_state.hw_vga_addr, g_vga_state.screen[screen_nbr].buffer,
         VGA_SCREEN_SIZE * sizeof(vga_char));
  return 1;
}

int vga_screen_clear(uint8_t screen_nbr) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;

  screen = &g_vga_state.screen[screen_nbr];
  // reset the cursor
  screen->cursor.column = 0;
  screen->cursor.row = 0;
  screen->attributes = (vga_attributes){0};
  // clear the screen
  bzero(screen->buffer, VGA_SCREEN_SIZE * sizeof(vga_char));

  return 1;
}
