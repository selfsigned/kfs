#include "cmds_debug.h"
#include "../../klibc/get_line.h"
#include "../../klibc/libc.h"
#include "../../syscalls/syscalls.h"

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

int cmd_intsyscall(uint8_t screen_nbr, int ac, char **av) {
  vga_info screen_info = {.screen = screen_nbr, .print = true};
  char input[5] = {0};
  int eax = 0;

  (void)ac, (void)av;
  vga_printf(screen_info, "EAX(0..%d)> ", SYSCALL_NBR - 1);
  if (!get_line(input, screen_nbr, sizeof(input) - sizeof(char))) { //-1 to \0
    return 1;
  }
  eax = atoi(input);
  if (eax < 0) {
    vga_printf(screen_info, "Error: Selection has to be positive");
    return 1;
  } else if (eax >= SYSCALL_NBR) {
    vga_printf(screen_info, "Error: syscall %d doesn't exist", eax);
    return 1;
  }

  __asm__ volatile("INT $0x80" ::"a"(eax) :);
  __asm__ volatile("MOV %0,%%eax" : "=a"(eax) :);
  vga_printf(screen_info, "Syscall returned value %d", eax);
  return 0;
}

int cmd_mode(uint8_t screen_nbr, int ac, char **av) {
  vga_info screen_info = {.screen = screen_nbr, .print = true};
  uint32_t cr0 = 0;

  (void)ac, (void)av;
  __asm__ volatile("MOV %%CR0, %0" : : "r"(cr0));
  vga_printf(screen_info, "System is running in %s mode. CR0:%b",
             (cr0 & 1) ? "protected" : "real", cr0);
  return 0;
}
