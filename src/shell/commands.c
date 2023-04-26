#include "cmds/cmd_greet.h"
#include "cmds/cmd_help.h"
#include "cmds/cmd_hexdump.h"
#include "cmds/cmd_kbd.h"
#include "cmds/cmd_stack.h"
#include "cmds/cmds_power.h"
#include "cmds/cmds_vgatest.h"
#include "shell.h"

#ifdef DEBUG
#include "cmds/cmds_debug.h"
#endif
// Available shell commands are defined here

struct shell_cmd g_shell_cmds[] = {
    {0, "help", cmd_help, {CMD_HELP_SMSG, CMD_HELP_LMSG}},
    {0, "greet", cmd_greet, {CMD_GREET_SMSG, CMD_GREET_LMSG}},

    // cmds_vgatest
    {0, "test_box", cmd_test_box, {CMD_TEST_BOX_SMSG, CMD_TEST_BOX_LMSG}},
    {0,
     "test_ipsum",
     cmd_test_ipsum,
     {CMD_TEST_IPSUM_SMSG, CMD_TEST_IPSUM_LMSG}},
    {0, "test_vga", cmd_test_vga, {CMD_TEST_VGA_SMSG, CMD_TEST_VGA_LMSG}},
    {0,
     "test_colors",
     cmd_test_colors,
     {CMD_TEST_COLORS_SMSG, CMD_TEST_COLORS_LMSG}},
    {0,
     "test_cp437",
     cmd_test_cp437,
     {CMD_TEST_CP437_SMSG, CMD_TEST_CP437_LMSG}},

    // hexdump
    {0, "hexdump", cmd_hexdump, {CMD_HEXDUMP_SMSG, CMD_HEXDUMP_LMSG}},

    // stack
    {0, "stack", cmd_stack, {CMD_STACK_SMSG, CMD_STACK_LMSG}},

    // keyboard
    {0, "kbd", cmd_kbd, {CMD_KBD_SMSG, CMD_KBD_LMSG}},

#ifdef DEBUG
    {0, "int0", cmd_int0, {CMD_INT0_SMSG, CMD_INT_LMSG}},
    {0, "int3", cmd_int3, {CMD_INT3_SMSG, CMD_INT_LMSG}},
    {0, "intspurious", cmd_intspurious, {CMD_INTSPURIOUS_SMSG, CMD_INT_LMSG}},
    {0, "intsyscall", cmd_intsyscall, {CMD_INTSYSCALL_SMSG, CMD_INT_LMSG}},
#endif

    // cmds_power
    {0, "reboot", cmd_reboot, {CMD_REBOOT_SMSG, CMD_REBOOT_LMSG}},

    {.flags = SHELL_CMD_END},
};
