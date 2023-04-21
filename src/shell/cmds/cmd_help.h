#ifndef CMD_HELP_H
#define CMD_HELP_H
// Shell help command

#include "../shell.h"

#define CMD_HELP_SMSG "Display this screen or detailed help"

#define CMD_HELP_LMSG                                                          \
  "To display all the available commands and their use:\n"                     \
  ">help\n\n"                                                                  \
  "To get more informations about a command:\n"                                \
  ">help <cmd>"

/// @brief Display the help messages for shell commands
/// @param screen_nbr screen to output too
/// @param ac shell argc
/// @param av shell argv
/// @return return code for cmd
int cmd_help(uint8_t screen_nbr, int ac, char *av[]);

#endif
