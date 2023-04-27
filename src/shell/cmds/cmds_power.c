#include "cmds_power.h"
#include "../../drivers/keyboard/ps2.h"
#include "../../klibc/libc.h"

int cmd_reboot(uint8_t screen_nbr, int ac, char **av) {
  (void)screen_nbr, (void)ac, (void)av;
  outb(KBD_IO_COMMAND_REGISTER, KBD_IO_SHUTDOWN);
  return 1;
}
