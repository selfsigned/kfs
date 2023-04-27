#ifndef CMD_STACK_H
#define CMD_STACK_H
// Stack printing command (required by KFS-2)

#include "../../debug/stack.h"
#include "../shell.h"

/// max number of stackframes to step into by default
#define STACKFRAME_NBR_DEFAULT 23

#define CMD_STACK_SMSG "Displays the stack"
#define CMD_STACK_LMSG                                                         \
  "Usage: stack [max stack frame nbr]\n"                                       \
  "The command will show the current stack frames and prompt the user which "  \
  "frame to display. Enter C^c not to choose any."

/// @brief displays the stack frame
int cmd_stack(uint8_t screen_nbr, int ac, char **av);

#endif
