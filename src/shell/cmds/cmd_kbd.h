#ifndef CMD_KBD_H
#define CMD_KBD_H
// Keyboard layout changer

#include "../shell.h"

#define CMD_KBD_SMSG "Change the keyboard layout"
#define CMD_KBD_LMSG "Prompts the user for a kbd layout"

/// @brief change the keyboard layout
int cmd_kbd(uint8_t screen_nbr, int ac, char **av);

#endif
