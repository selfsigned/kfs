#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/keyboard/keyboard.h"
#include "drivers/vga/vga.h"
#include "kernel.h"
#include "klibc/libc.h"

#include "screens/screens.h"
#include "test/test.h"

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

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  volatile int *memory_signature = (void *)SIGNATURE_ADDRESS;

  // initialized the vga driver with an history size of 16*25 rows
  vga_init(16, (uint16_t *)SCREEN_BUFFER_ADDR);
  test_vga(9, false,
           false); // write tests on scr 9 w/o scrolling and print them
  screen_lorem_ipsum(1, 2);

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;

  // screen management
  uint8_t current_screen = 0;
  int scroll_state = 0;
  bool isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

  while (42) {
    scancode code = 0;
    char c = 0;

    if ((code = kbd_get())) {
      c = kbd_code_to_ascii(code);

      if (isprint(c) && current_screen == 0) {
        vga_printf((vga_info){}, "%C", c);
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
          if (!current_screen)
            vga_screen_clear(0);
          break;
        case KBD_ENTER:
          if (!current_screen)
            vga_printf((vga_info){}, "\n");
          break;
        case KBD_TAB:
          if (!current_screen)
            vga_printf((vga_info){}, "    ");
          break;
        default:
          break;
        }
      }
      isnotup = vga_screen_show_scrolled(current_screen, scroll_state);
    }
  }
}
