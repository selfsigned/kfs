#include "../drivers/keyboard/keyboard.h"
#include "../drivers/vga/vga.h"
#include "../klibc/libc.h"
#include "screens.h"

static int set_scroll(uint8_t screen_nbr, scancode code, int scroll_state,
                      bool isnotup) {
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
    if (scroll_state == -1) {
      scroll_state = vga_screen_getscrolloffset(screen_nbr) - 1;
    }
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
    if ((isprint(kbd.ascii) || kbd.key == KBD_SPACE) &&
        current_screen == note_screen) {
      vga_printf((vga_info){.screen = note_screen}, "%C", kbd.ascii);
      scroll_state = 0;
    } else {
      // control characters
      switch (kbd.key) {
      case KBD_F1 ... KBD_F10:
        // switch screen
        current_screen = kbd.key - KBD_F1;
        scroll_state = 0;
        break;
      case KBD_HOME ... KBD_PAGE_DOWN:
        // scroll
        scroll_state =
            set_scroll(current_screen, kbd.key, scroll_state, isnotup);
        break;
      // control chars
      case KBD_ENTER:
        if (current_screen == note_screen)
          vga_printf((vga_info){.screen = note_screen}, "\n");
        break;
      case KBD_TAB:
        if (current_screen == note_screen)
          vga_printf((vga_info){.screen = note_screen}, "    ");
        break;
      default:
        break;
      }
    }

    if (current_screen == note_screen)
      vga_screen_setvgacursor(note_screen, true);
    isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

    __asm__("HLT"); // halt until next interrupt
  }
}
