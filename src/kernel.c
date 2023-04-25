#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/keyboard/keyboard.h"
#include "drivers/vga/vga.h"
#include "hardware/gdt/gdt.h"
#include "hardware/interrupts/interrupts.h"
#include "kernel.h"
#include "klibc/libc.h"

#include "cp437.h"
#include "log.h"
#include "shell/shell.h"

static int set_scroll(uint8_t screen_nbr, kbd_scancode code, int scroll_state,
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
  case KBD_PAGE_DOWN:
    if (scroll_state > 0)
      for (int i = 10; scroll_state && i; scroll_state--, i--)
        ;
    if (scroll_state == -1)
      scroll_state = vga_screen_getscrolloffset(screen_nbr) - 10;
    break;
  default:
  }
  return scroll_state;
}

void screen_loop() {
  shell_state shell[SHELL_NBR] = {0}; // shell internal state
                                      // !! WILL BREAK THE STACK IF TOO BIG!!
  uint8_t current_screen = 0;
  int scroll_state = 0;
  bool isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

  // initialize shells
  for (size_t i = 0; i < SHELL_NBR; ++i) {
    shell[i].screen = i;
  }

  while (true) {
    if (kbd.status == KEY_RELEASED)
      goto halt;

    if (isprint(kbd.ascii))
      scroll_state = 0;
    else {
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
      default:
        break;
      }
    }

    // TODO check if more conditions needed
    if (current_screen < SHELL_NBR && current_screen != LOG_SCREEN) {
      shell_run(&shell[current_screen], kbd);
    }

    // set cursor
    vga_screen_setvgacursor(current_screen, current_screen != LOG_SCREEN);
    // display and check if at top of history
    isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

  halt:
    __asm__ volatile("HLT"); // halt until next interrupt
  }
}

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  volatile int *memory_signature = (void *)SIGNATURE_ADDRESS;

  // initialized the vga driver and set screens
  vga_init(256, (uint16_t *)SCREEN_BUFFER_ADDR);

  // flat-memory model
  gdt_init();

  // initialize interrupts
  int_init();

  // initalize the PS2 Keyboard controller
  kbd_init();

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;
  INFO_MSG("Kernel canary %p set at %p", SIGNATURE_VALUE, memory_signature);

  // call the home screen loop
  screen_loop();
}
