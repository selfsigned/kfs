#include "vga_internal.h"

uint16_t *vga_set_buffer_addr = (uint16_t *)VGA_HW_BUFFER_ADDR;
uint16_t vga_set_buffer_history_size = (uint16_t)VGA_BUFFER_HISTORY_SIZE;
vga_global_info g_vga_state = {};

// private

/// @brief initialize the vga driver
int vga_buffer_init() {
  static bool init = false;

  if (vga_set_buffer_history_size == 0)
    return -1;

  // initalizing screens in RAM
  if (!init || (g_vga_state.buffer.addr != vga_set_buffer_addr) ||
      (g_vga_state.buffer.screens != vga_set_buffer_history_size)) {
    // set the global state
    g_vga_state.vga_addr = (uint16_t *)VGA_HW_ADDR;
    g_vga_state.buffer.addr = vga_set_buffer_addr;
    g_vga_state.buffer.screens = vga_set_buffer_history_size;
    g_vga_state.buffer.size = (VGA_SCREEN_SIZE * g_vga_state.buffer.screens);

    // populate addresses and clean them
    for (unsigned int i = 0; i < VGA_SCREEN_MAX; ++i) {
      vga_char *head = (vga_char *)(g_vga_state.buffer.addr +
                                    (i * g_vga_state.buffer.size) + i);

      g_vga_state.screen[i] = (vga_screen_info){
          .buffer.head = head,
          .buffer.tail = head + g_vga_state.buffer.size,
      };
      // set the screen viewpoint to head and clean the screen
      g_vga_state.screen[i].buffer.pos = g_vga_state.screen[i].buffer.head;
      bzero(g_vga_state.screen[i].buffer.head,
            g_vga_state.buffer.size * sizeof(vga_char));
      // DEBUG memory areas
      // memset(g_vga_state.screen[i].buffer.head, ('a' + i),
      //        g_vga_state.buffer.size * sizeof(vga_char));
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

  // insert newline
  if (insert_newline && info->row <= VGA_ROW) {
    info->column = 0;
    info->row++;
    if (!info->nocursor) {
      screen->cursor.column = 0;
      screen->cursor.row++;
    }
  }

  if (info->row >= VGA_ROW) {
    if (info->noscroll)
      return 0;

    // cycle buffer at end of scroll
    if ((screen->buffer.pos + VGA_SCREEN_SIZE) >= screen->buffer.tail) {
      vga_char tmp[VGA_SCREEN_SIZE] = {0};

      memcpy(tmp, screen->buffer.pos, (VGA_SCREEN_SIZE) * sizeof(vga_char));
      bzero(screen->buffer.head, (VGA_SCREEN_SIZE) * sizeof(vga_char));
      screen->buffer.pos = screen->buffer.head;
      memcpy(screen->buffer.head, tmp + VGA_COL,
             (VGA_SCREEN_SIZE - VGA_COL) * sizeof(vga_char));
    } else {
      screen->buffer.pos += VGA_COL;
    }

    info->row--;
    info->column = 0;
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

  if (vga_buffer_init() < 0)
    return 0;

  // load attributes and cursor state
  screen = vga_set_cursor(info, false);
  vgac = (vga_char){.c = c,
                    .color = (!info->noattributes) ? screen->attributes
                                                   : (vga_attributes){}};

  // Wrap to next line
  if (info->column >= VGA_COL) {
    if (info->nowrap)
      return 0;

    if (info->wrapchar)
      wrap_char = info->wrapchar;
    info->column = 0;
    info->row++;
    result++;
    if (!info->nowrapchar)
      vga_buffer_writechar(info, wrap_char);
  }

  // set character in buffer
  position = (info->row * VGA_COL) + info->column;
  screen->buffer.pos[position] = vgac;

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

  memcpy(g_vga_state.vga_addr, g_vga_state.screen[screen_nbr].buffer.pos,
         VGA_SCREEN_SIZE * sizeof(vga_char));
  return 1;
}

int vga_screen_show_scrolled(uint8_t screen_nbr, uint32_t rows,
                             bool scroll_overlay) {
  vga_screen_info *screen;
  vga_char *target;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;

  // Overlay not implemented yet
  screen = &g_vga_state.screen[screen_nbr];
  target = screen->buffer.pos - (rows * VGA_COL);
  if (target < screen->buffer.head)
    target = screen->buffer.head;

  memcpy(g_vga_state.vga_addr, target, VGA_SCREEN_SIZE * sizeof(vga_char));

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
  bzero(screen->buffer.pos, VGA_SCREEN_SIZE * sizeof(vga_char));

  return 1;
}
