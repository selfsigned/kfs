#ifndef CMDS_DEBUG
#define CMDS_DEBUG
/// various potentially dangerous debug commands

#include "../shell.h"

#define CMD_INT0_SMSG "DEBUG Triggers exception interrupt 0"
#define CMD_INT3_SMSG "DEBUG Triggers exception interrupt 3 (Dangerous!)"
#define CMD_INTSPURIOUS_SMSG "DEBUG Triggers spurious keyboard interrupt"
#define CMD_INTSYSCALL_SMSG                                                    \
  "DEBUG Triggers syscall interrupt (Not implemented, Dangerous!)"
#define CMD_INT_LMSG                                                           \
  "IDT->0-31:exceptions 32-48:HW interrupts 0x80:syscalls ...:sw interrupts"

/// @brief trigger an interrupt
int cmd_int0(uint8_t screen_nbr, int ac, char **av);
int cmd_int3(uint8_t screen_nbr, int ac, char **av);
int cmd_intspurious(uint8_t screen_nbr, int ac, char **av);
int cmd_intsyscall(uint8_t screen_nbr, int ac, char **av);

#define CMD_MODE_SMSG "Checks the cpu current mode (PE)"
#define CMD_MODE_LMSG                                                          \
  "Real mode is the 16 bit compatibility mode, Protected mode allows the use " \
  "of all the cpu features"

/// @brief check cpu mode
int cmd_mode(uint8_t screen_nbr, int ac, char **av);

#endif
