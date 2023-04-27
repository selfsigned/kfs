#include "cmds_debug.h"
#include "../../klibc/libc.h"

// INT only takes an immediate value sadly
#define INTERRUPT_CMD(N, INT_NBR)                                              \
  int cmd_##N(uint8_t screen_nbr, int ac, char **av) {                         \
    (void)ac, (void)av;                                                        \
    vga_info screen_info = {.screen = screen_nbr};                             \
    vga_printf(screen_info, "Triggering INT $" #INT_NBR);                      \
    __asm__ volatile("INT $" #INT_NBR ::);                                     \
    return 0;                                                                  \
  }

INTERRUPT_CMD(int0, 0)
INTERRUPT_CMD(int3, 3)
INTERRUPT_CMD(intspurious, 33)
INTERRUPT_CMD(intsyscall, 0x80)
