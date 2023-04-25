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
  vga_printf(screen_info, "%s\n%35s\n", KFS_LOGO, VERSION); // logo && version
  // back to white
  vga_screen_setattributes(screen_nbr, (vga_attributes){.fg = VGA_COLOR_WHITE});

  vga_printf(screen_info,
             "\nF1...F%u: Select a screen(F%u:log) %C %C Home End: Scroll up "
             "and down\n",
             SHELL_NBR, LOG_SCREEN, CP437_UP_ARROW, CP437_DOWN_ARROW);

  vga_printf(screen_info,
             "\nWelcome to " OS_NAME
             " on screen %u! Type 'help' to see available commands\n",
             screen_nbr);

  return 0;
}
