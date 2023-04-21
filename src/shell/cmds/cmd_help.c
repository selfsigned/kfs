#include "cmd_help.h"

int cmd_help(uint8_t screen_nbr, int ac, char *av[]) {
  vga_info screen_info = {.screen = screen_nbr, .nowrapchar = true};
  struct shell_cmd *cmd = NULL;

  // display short help for every cmd if no params
  if (ac < 1) {
    vga_printf(screen_info, "Commands:\n");
    for (struct shell_cmd *cmd = g_shell_cmds; !(cmd->flags & SHELL_CMD_END);
         cmd++)
      vga_printf(screen_info, "%-15s%s\n", cmd->name, cmd->help.smsg);
    vga_printf(screen_info, "\nTo get detailed informations: help <cmd>");
    return 0;
  }

  // display short help and long help
  while (ac--) {
    if ((cmd = shell_get_cmd(av[ac]))) {
      vga_printf(screen_info, "%s - %s:\n%s", cmd->name, cmd->help.smsg,
                 cmd->help.lmsg);
    }
  }
  return 0;
}
