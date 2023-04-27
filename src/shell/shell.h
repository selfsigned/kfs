#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../drivers/keyboard/keyboard.h"

//  _
// /  _ ._ _ ._ _  _.._  _| _
// \_(_)| | || | |(_|| |(_|_>

/// Maximum command name length
#define SHELL_MAX_CMDNAME 25

/// Maximum short help message lenght
#define SHELL_MAX_SHELP 80

/// Maximum long help message lenght
#define SHELL_MAX_LHELP 400

enum shell_cmd_flags {
  SHELL_CMD_END = 128,
};

typedef struct shell_cmd {
  uint8_t flags;
  char name[SHELL_MAX_CMDNAME]; /// name of the command
  // TODO maybe optional screen state argument
  int (*func)(uint8_t screen_nbr, int argc, char **argv); /// command
  struct {
    char smsg[SHELL_MAX_SHELP]; /// short help message
    char lmsg[SHELL_MAX_LHELP]; /// long help message
  } help;
} shell_cmd;

// ___
//  | ._ ._   _|_
// _|_| ||_)|_||_
//       |

/// Max shell command line input length
#define SHELL_MAX_INPUT 2000

/// Maximum number of arguments
#define SHELL_MAX_ARGV 9

typedef struct shell_state {
  uint8_t screen; /// Screen nbr the shell is @, initialize this
  bool greeted;   /// false if the shell hasn't greeted the user yet

  struct {
    char buf[SHELL_MAX_INPUT]; /// Shell input buffer
    size_t idx;                /// Shell input buffer index
  } input;
  shell_cmd *last_cmd; /// last command executed
  uint8_t return_code;
} shell_state;

//  /\ ._ o
// /--\|_)|
//     |

#define SHELL_PS1 "> "

/// Where shell commands are registered, add new commends to commands.c.
/// Last commands should be empty except for the  SHELL_CMD_LAST flag
extern struct shell_cmd g_shell_cmds[];

/// @brief Interpret a keystroke in the shell
/// @param state State of the chosen shell
/// @param key Keycode/ascii to send to the shell
void shell_run(shell_state *state, struct kbd_state key);

/// @brief Get first shell cmd that matches `name`
/// @param name name of the shell command
/// @return NULL if not found, address of the shell cmd definition otherwise
shell_cmd *shell_get_cmd(char *name);

#endif
