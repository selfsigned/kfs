#include "cmd_kbd.h"
#include "../../drivers/keyboard/layout.h"
#include "../../klibc/get_line.h"
#include "../../klibc/libc.h"

int cmd_kbd(uint8_t screen_nbr, int ac, char **av) {
  vga_info screen_info = {.screen = screen_nbr, .print = true};
  char input[2] = {0};
  int input_nbr;
  size_t i;

  (void)ac, (void)av;
  for (i = 0; kbd_available_layouts[i].layout; ++i) {
    vga_printf(screen_info, "\t%u) %s\n", i,
               kbd_available_layouts[i].layout_name);
  }
  // Prompt the user for the keyboard layout
  vga_printf(screen_info, "Layout(nbr)> ");
  if (!get_line(input, screen_nbr, sizeof(input) - sizeof(char))) { //-1 to \0
    return 1;
  }
  input_nbr = atoi(input);
  if (input_nbr < 0) {
    vga_printf(screen_info, "Error: Selection has to be positive");
    return 1;
  } else if ((size_t)input_nbr >= i) {
    vga_printf(screen_info, "Error: Selected layout does not exist");
    return 1;
  }

  kbd_layout = kbd_available_layouts[input_nbr].layout;
  return 0;
}
