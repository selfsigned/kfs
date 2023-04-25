#include "get_line.h"
#include "../drivers/keyboard/keyboard.h"
#include "../drivers/vga/vga.h"
#include "libc.h"

size_t get_line(char *dest, int screen, size_t maxlen) {
  bool enable_display = screen >= 0 && screen < UINT8_MAX;
  vga_info screen_info = {.screen = screen, .print = true};
  size_t nbr_read = 0;

  // ignore residual key
  if (kbd.key == KBD_ENTER)
    while (kbd.key == KBD_ENTER)
      __asm__ volatile("HLT"); // halt until next interrupt
  // loop until '\n' or 'C^c' are pressed
  while (true) {
    if (kbd.status == KEY_RELEASED)
      goto halt;

    if (isprint(kbd.ascii)) {
      if (kbd.mod.ctrl && kbd.ascii == 'c') {
        if (enable_display)
          vga_printf(screen_info, "\n");
        bzero(dest, nbr_read);
        return 0;
      } else if (nbr_read < maxlen) {
        dest[nbr_read++] = kbd.ascii;
        if (enable_display)
          vga_printf(screen_info, "%c", kbd.ascii);
      }
    } else
      switch (kbd.key) {

      case KBD_ENTER:
        if (enable_display)
          vga_printf(screen_info, "\n");
        return nbr_read;

      case KBD_BACKSPACE:
        if (nbr_read) {
          dest[--nbr_read] = '\0';
          if (enable_display)
            vga_printf(screen_info, "\b");
        }
        break;

      default:
        break;
      }

  halt:
    __asm__ volatile("HLT"); // halt until next interrupt
  }
}
