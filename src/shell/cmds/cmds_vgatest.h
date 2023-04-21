#ifndef CMDS_VGATEST_H
#define CMDS_VGATEST_H
// VGA driver test commands

#include "../../cp437.h"
#include "../shell.h"

#define CMD_TEST_BOX_USAGE "Usage: cmd_test_box <width> <height>"
#define CMD_TEST_BOX_SMSG "Draw a text box (broken-ish)"
#define CMD_TEST_BOX_LMSG CMD_TEST_BOX_USAGE

/// @brief Create a test box
int cmd_test_box(uint8_t screen_nbr, int argc, char **argv);

#define CMD_TEST_IPSUM_SMSG "Displays a lenghty lorem ipsum"
#define CMD_TEST_IPSUM_LMSG                                                    \
  "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur "        \
  "finibus ultrices elit et consequat. Integer commodo justo lectus, at "      \
  "mollis velit congue ac. Cras consequat mi et justo faucibus pretium. Duis " \
  "quis rutrum risus, tempus tristique ipsum. Nulla facilisi"

/// @brief everyone's favorite, lorem ipsum
int cmd_test_ipsum(uint8_t screen_nbr, int argc, char **argv);

#define CMD_TEST_VGA_SMSG "Displays a demo of the printf/vga driver"
#define CMD_TEST_VGA_LMSG                                                      \
  "This will clear the screen and print stuff in color and above"
/// @brief printf/vga driver demo
int cmd_test_vga(uint8_t screen_nbr, int argc, char **argv);

#define CMD_TEST_COLORS_SMSG "Displays all the possible VGA text mode colors"
#define CMD_TEST_COLORS_LMSG " "

/// @brief show all possible vga text mode colors
int cmd_test_colors(uint8_t screen_nbr, int argc, char **argv);

#define CMD_TEST_CP437_SMSG "Displays all the CP437 charset"
#define CMD_TEST_CP437_LMSG                                                    \
  "Code page 437 (CCSID 437) is the character set of the original IBM PC "     \
  "(personal computer)."

/// @brief show all cp437 characters
int cmd_test_cp437(uint8_t screen_nbr, int argc, char **argv);

#endif
