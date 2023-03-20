#include "vga_internal.h"

vga_global_info g_vga_state = {};

// private //

static bool vga_set_crtc_cursor(uint8_t screen_nbr) {
  vga_screen_info screen;
  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = g_vga_state.screen[screen_nbr];
  if (screen.cursor.column >= VGA_COL && screen.cursor.row >= VGA_ROW)
    return false;
  screen.buffer.pos[screen.cursor.row * VGA_COL + screen.cursor.column + 1] =
      (vga_char){.color = screen.attributes};
  // TODO param for cursor size?
  vga_crtc_enable_cursor(VGA_CURSOR_HEIGHT);
  vga_crtc_set_cursor(screen.cursor.column, screen.cursor.row);

  return true;
}

vga_screen_info *vga_set_cursor(vga_info *info, bool insert_newline) {
  vga_screen_info *screen = &g_vga_state.screen[info->screen];

  screen->cursor.vga = info->nocursor;
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

  // scroll
  if (info->row >= VGA_ROW) {
    if (info->noscroll)
      return 0;

    // TODO cycle buffer at end of scroll
    if ((screen->buffer.pos + VGA_SCREEN_SIZE) >= screen->buffer.tail) {
      // vga_char tmp[VGA_SCREEN_SIZE] = {0};

      // memcpy(tmp, screen->buffer.pos, (VGA_SCREEN_SIZE) * sizeof(vga_char));
      // bzero(screen->buffer.head, (VGA_SCREEN_SIZE) * sizeof(vga_char));
      // screen->buffer.pos = screen->buffer.head;
      // memcpy(screen->buffer.head, tmp + VGA_COL,
      //        (VGA_SCREEN_SIZE - VGA_COL) * sizeof(vga_char));
      bzero(screen->buffer.head, g_vga_state.buffer.size * sizeof(vga_char));
      vga_screen_clear(0);
      vga_screen_setcursorpos(info->screen, 0, 0);
      screen->buffer.pos = screen->buffer.head;
    } else {
      screen->buffer.pos += VGA_COL;
    }

    // carry over color attributes
    if (info->scrollattributes)
      for (uint8_t i = 0; i < VGA_COL; ++i)
        screen->buffer.pos[VGA_SCREEN_SIZE - VGA_COL + i].color =
            screen->attributes;

    info->row = 24;
  }

  return screen;
}

int vga_buffer_writechar(vga_info *info, const unsigned char c) {
  int result = 0;
  unsigned char wrap_char = VGA_WRAP_DEFAULT_CHAR;
  vga_char vgac;           // character to write to screen
  uint16_t position;       // position on screen
  vga_screen_info *screen; // chosen screen

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

int vga_buffer_write(vga_info *info, const unsigned char *s, bool is_ascii) {
  const unsigned char *str = s;
  size_t result = 0;

  while (*str) {
    // TODO: \t
    if (is_ascii && *str == '\n')
      vga_set_cursor(info, true);
    else
      result += vga_buffer_writechar(info, *str);
    str++;
  }
  return result;
}

// public //

bool vga_init(size_t history_size, uint16_t *buffer_addr) {
  static bool init = false;

  // initalizing screens in RAM
  if (history_size != 0 &&
      (!init || (g_vga_state.buffer.addr != buffer_addr) ||
       (g_vga_state.buffer.history_size != history_size))) {
    // set the global state
    g_vga_state.vga_addr = (uint16_t *)VGA_HW_ADDR;
    g_vga_state.buffer.addr = buffer_addr;
    g_vga_state.buffer.history_size = history_size;
    g_vga_state.buffer.size =
        (VGA_SCREEN_SIZE * g_vga_state.buffer.history_size);

    // populate addresses and clean them
    for (unsigned int i = 0; i < VGA_SCREEN_MAX; ++i) {
      vga_char *head = (vga_char *)(g_vga_state.buffer.addr +
                                    (i * g_vga_state.buffer.size) + i);

      g_vga_state.screen[i] = (vga_screen_info){
          .buffer.head = head,
          .buffer.tail = head + g_vga_state.buffer.size,
          .attributes = (vga_attributes){.fg = VGA_COLOR_LIGHT_GREY}};
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

  return init;
}

// Getters / Setters

bool vga_screen_getcursorpos(uint8_t screen_nbr, uint8_t *column,
                             uint8_t *row) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  *column = screen->cursor.column;
  *row = screen->cursor.row;
  return true;
}

bool vga_screen_setcursorpos(uint8_t screen_nbr, uint8_t column, uint8_t row) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX || column >= VGA_COL || row >= VGA_ROW)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  screen->cursor.column = column;
  screen->cursor.row = row;
  return true;
}

bool vga_screen_setattributes(uint8_t screen_nbr, vga_attributes attributes) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  screen->attributes = attributes;

  return true;
}

bool vga_screen_setvgacursor(uint8_t screen_nbr, bool enable_cursor) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  screen->cursor.vga = enable_cursor;

  return true;
}

bool vga_screen_fillattributes(uint8_t screen_nbr, vga_attributes attributes) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  screen->attributes = attributes;
  for (size_t i = 0; i < VGA_SCREEN_SIZE; ++i)
    screen->buffer.pos[i].color = attributes;
  return true;
}

bool vga_screen_fillbackground(uint8_t screen_nbr,
                               enum vga_color background_color) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  screen->attributes.bg = background_color;
  for (size_t i = 0; i < VGA_SCREEN_SIZE; ++i)
    screen->buffer.pos[i].color.bg = background_color;
  return true;
}

// Show

bool vga_screen_show(uint8_t screen_nbr) {
  vga_screen_info screen;
  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = g_vga_state.screen[screen_nbr];
  if (screen.cursor.vga) {
    vga_set_crtc_cursor(screen_nbr);
  } else
    vga_crtc_disable_cursor();

  memcpy(g_vga_state.vga_addr, screen.buffer.pos,
         VGA_SCREEN_SIZE * sizeof(vga_char));
  return true;
}

int vga_screen_show_scrolled(uint8_t screen_nbr, int rows) {
  vga_screen_info *screen;
  vga_char *target;
  bool isnotup = true;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;

  // Overlay not implemented yet
  screen = &g_vga_state.screen[screen_nbr];
  target = screen->buffer.pos - (rows * VGA_COL);
  if (rows < 0 || target < screen->buffer.head) {
    target = screen->buffer.head;
    isnotup = false;
  }

  // disabled cursor if scrolled up
  if (screen->cursor.vga && !rows) {
    // TODO param for cursor size? and set attrs to current instead of -1
    vga_set_crtc_cursor(screen_nbr);
  } else
    vga_crtc_disable_cursor();

  memcpy(g_vga_state.vga_addr, target, VGA_SCREEN_SIZE * sizeof(vga_char));
  return isnotup;
}

int vga_screen_getscrolloffset(uint8_t screen_nbr) {
  vga_screen_info screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return -42;
  screen = g_vga_state.screen[screen_nbr];
  return (screen.buffer.pos - screen.buffer.head) / 80;
}

// Clear

bool vga_screen_clear(uint8_t screen_nbr) {
  vga_screen_info *screen;

  if (screen_nbr >= VGA_SCREEN_MAX)
    return false;

  screen = &g_vga_state.screen[screen_nbr];
  // reset the cursor
  screen->cursor.column = 0;
  screen->cursor.row = 0;
  screen->attributes = (vga_attributes){0};
  // clear the screen
  bzero(screen->buffer.pos, VGA_SCREEN_SIZE * sizeof(vga_char));

  return true;
}
