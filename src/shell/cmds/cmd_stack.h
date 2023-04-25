#ifndef CMD_STACK_H
#define CMD_STACK_H
// Stack printing commands (required by KFS-2)

#include "../shell.h"

/// max number of stackframes to step into by default
#define STACKFRAME_NBR_DEFAULT 20

#define CMD_STACK_SMSG "Displays the stack"
#define CMD_STACK_LMSG "TODO explain"

/// @brief displays the stack frame
int cmd_stack(uint8_t screen_nbr, int ac, char **av);

#endif
