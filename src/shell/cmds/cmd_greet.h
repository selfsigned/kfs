#ifndef CMD_GREET_H
#define CMD_GREET_H

#include "../shell.h"

#define CMD_GREET_SMSG "Displays the welcome message"
#define CMD_GREET_LMSG "There's not much else to say"

/// @brief Display the help messages for shell commands
/// @param screen_nbr screen to output too
/// @param ac ignored
/// @param av ignored
/// @return return code for cmd
int cmd_greet(uint8_t screen_nbr, int ac, char **av);

#endif
