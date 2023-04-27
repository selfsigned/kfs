#include "cmd_greet.h"
// display greeting welcome message

#include "../../cp437.h"
#include "../../kernel.h"

int cmd_greet(uint8_t screen_nbr, int ac, char **av) {
  vga_info screen_info = {.screen = screen_nbr};

  // shut up unused warnings
  (void)ac;
  (void)av;

  // paint it red
  vga_screen_setattributes(screen_nbr, (vga_attributes){.fg = VGA_COLOR_RED});
  vga_printf(screen_info, "%s%s\n", KFS_LOGO, VERSION); // logo && version
  // back to white
  vga_screen_setattributes(screen_nbr, (vga_attributes){.fg = VGA_COLOR_WHITE});

  vga_printf(screen_info,
             "\nWelcome to " OS_NAME " on screen %u!\n"
             "Type 'help' to see available commands",
             screen_nbr);

  return 0;
}
