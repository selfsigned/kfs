#include "../drivers/keyboard/keyboard.h"
#include "../drivers/vga/vga.h"
#include "../klibc/libc.h"
#include "screens.h"

static int set_scroll(scancode code, int scroll_state, bool isnotup) {
  switch (code) {
  case KBD_HOME:
    scroll_state = -1;
    break;
  case KBD_END:
    scroll_state = 0;
    break;
  case KBD_CURSOR_UP:
    if (isnotup)
      scroll_state++;
    break;
  case KBD_CURSOR_DOWN:
    if (scroll_state > 0)
      scroll_state--;
    break;
  default:
  }
  return scroll_state;
}

void screen_loop(uint8_t home_screen, uint8_t note_screen) {
  uint8_t current_screen = home_screen;
  int scroll_state = 0;
  bool isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

  // TODO function or charmap for the homepage print
  while (42) {
    scancode code = 0;
    char c = 0;

    if ((code = kbd_get())) {
      c = kbd_code_to_ascii(code);

      if (isprint(c) && current_screen == note_screen) {
        vga_printf((vga_info){.screen = note_screen}, "%C", c);
        scroll_state = 0;
      } else {
        // control characters
        switch (code) {
        case KBD_F1 ... KBD_F10:
          // switch screen
          current_screen = code - KBD_F1;
          scroll_state = 0;
          break;
        case KBD_HOME ... KBD_PAGE_DOWN:
          // scroll
          scroll_state = set_scroll(code, scroll_state, isnotup);
          break;
        // control chars
        case KBD_DELETE:
          if (current_screen == note_screen)
            vga_screen_clear(note_screen);
          break;
        case KBD_ENTER:
          if (current_screen == note_screen)
            vga_printf((vga_info){.screen = note_screen}, "\n");
          break;
        case KBD_TAB:
          if (!current_screen)
            vga_printf((vga_info){.screen = note_screen}, "    ");
          break;
        default:
          break;
        }
      }
      isnotup = vga_screen_show_scrolled(current_screen, scroll_state);
    }
  }
}
