#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/keyboard/keyboard.h"
#include "drivers/vga/vga.h"
#include "kernel.h"
#include "klibc/libc.h"

#include "cp437.h"
#include "screens/screens.h"
#include "test/test.h"

#define SCREEN_TOTAL 12

enum screen_numbers {
  HOME_SCREEN,
  NOTE_SCREEN,
  IPSUM_SCREEN,
  PRINTF_DEMO_SCREEN = 8,
  VGA_DEMO_SCREEN,
};

void screen_init(uint8_t screen_nbr) {
  struct panel {
    char title[40];
    void (*screen_init_func)(uint8_t);
  };
  struct panel screen[12] = {
      [HOME_SCREEN] = {"Home screen (current)"},
      [NOTE_SCREEN] = {"Note screen"},
      [IPSUM_SCREEN] = {"Lorem ipsum dolor sit amet", &screen_lorem_ipsum},
      [PRINTF_DEMO_SCREEN] = {"vga_printf() demo", &test_printf},
      [VGA_DEMO_SCREEN] = {"CP437 and Colors demo", &test_vga_cp437}};

  vga_screen_setattributes(screen_nbr, (vga_attributes){.fg = VGA_COLOR_RED});

  // logo
  vga_printf((vga_info){.screen = screen_nbr, .row = 2}, "%s\n", KFS_LOGO);

  // version
  vga_printf((vga_info){.screen = screen_nbr, .nowrap = true, .column = 35},
             "vers: %s\n", VERSION);

  // silly box
  // screen_create_box_light(screen_nbr, VGA_COL - 1, VGA_ROW - 1);

  vga_screen_setattributes(screen_nbr, (vga_attributes){.fg = VGA_COLOR_WHITE});

  // Print function keys and init screens
  for (uint8_t i = 0; i < SCREEN_TOTAL; ++i)
    if (screen[i].title[0]) {
      if (screen[i].screen_init_func)
        screen[i].screen_init_func(i);

      vga_printf((vga_info){.screen = screen_nbr, .column = 5}, "F%u: %s\n",
                 i + 1, screen[i].title);
    }

  vga_printf((vga_info){.screen = screen_nbr, .column = 5},
             "%C %C: Scroll up and down\n", CP437_UP_ARROW, CP437_DOWN_ARROW);
  vga_printf((vga_info){.screen = screen_nbr, .column = 5},
             "HOME: Scroll to the top of the screen\n");
  vga_printf((vga_info){.screen = screen_nbr, .column = 5},
             "END: Scroll to the bottom of the screen\n");

  vga_screen_show(screen_nbr);
}

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  volatile int *memory_signature = (void *)SIGNATURE_ADDRESS;

  // initialized the vga driver and set screens
  vga_init(16, (uint16_t *)SCREEN_BUFFER_ADDR);
  screen_init(HOME_SCREEN);

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;

  // call the home screen loop
  screen_loop(HOME_SCREEN, NOTE_SCREEN);
}
