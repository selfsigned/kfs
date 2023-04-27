#ifndef CMD_BRAINFUCK_H
#define CMD_BRAINFUCK_H
// Brainfuck interpreter

#include "../shell.h"

/// Spec asks for 30k
#define BRAINFUCK_TAPE_SIZE 30000

/// @brief a demo program entry
struct brainfuck_demo_tape {
  char name[30];
  char program[2000]; // might be too short
};

#define CMD_BRAINFUCK_SMSG "Brainfuck interpreter"
#define CMD_BRAINFUCK_LMSG                                                     \
  "Demo mode: Run the command without parameters to chose a demo program\n"    \
  "Interpreter: brainfuck <input>"

/// @brief runs a brainfuck program
int cmd_brainfuck(uint8_t screen_nbr, int ac, char **av);

#endif
