#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "drivers/keyboard/keyboard.h"
#include "drivers/vga/vga.h"
#include "kernel.h"
#include "klibc/libc.h"

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

void home_screen(uint8_t screen_nbr) {
  struct panel {
    char title[40];
  };
  struct panel screen_description[12] = {
      [HOME_SCREEN] = {"Home screen (current)"},
      [NOTE_SCREEN] = {"Note screen"},
      [IPSUM_SCREEN] = {"Lorem ipsum dolor sit amet"},
      [PRINTF_DEMO_SCREEN] = {"vga_printf() demo"},
      [VGA_DEMO_SCREEN] = {"CP437 and Colors demo"}};

  // logo
  vga_printf((vga_info){.screen = screen_nbr, .row = 2}, "%a%s%a\n",
             (vga_attributes){.fg = VGA_COLOR_RED}, KFS_LOGO,
             (vga_attributes){.fg = VGA_COLOR_LIGHT_GREY});
  // Function keys
  for (uint8_t i = 0; i < SCREEN_TOTAL; ++i)
    if (screen_description[i].title[0])
      vga_printf((vga_info){.screen = screen_nbr}, "     F%u: %s\n", i + 1,
                 screen_description[i].title);

  vga_screen_show(screen_nbr);
}

/// @brief The entrypoint of our kernel
void kernel_main(void) {
  volatile int *memory_signature = (void *)SIGNATURE_ADDRESS;

  // initialized the vga driver and set screens
  vga_init(16, (uint16_t *)SCREEN_BUFFER_ADDR);
  home_screen(HOME_SCREEN);
  screen_lorem_ipsum(IPSUM_SCREEN, 1);
  test_vga_cp437(VGA_DEMO_SCREEN);
  test_printf(PRINTF_DEMO_SCREEN);

  // Set the kernel signature
  *memory_signature = SIGNATURE_VALUE;

  // call the home screen loop
  screen_loop(HOME_SCREEN, NOTE_SCREEN);
}
