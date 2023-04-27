#include "cmd_brainfuck.h"
#include "../../klibc/get_line.h"
#include "../../klibc/libc.h"

// demo programs
const struct brainfuck_demo_tape brainfuck_demo_tapes[] = {
    {.name = "hello world!",
     .program =
         "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++."
         "<<+++++++++++++++.>.+++.------.--------.>+.>."},
    {.name = "ascii", .program = ".+[.+]"},
    {.name = "rot13 (can't exit)",
     .program =
         "-,+[-[>>++++[>++++++++<-]<+<-[>+>+>-[>>>]<[[>+<-]>>+>]<<<<<-]]>>>[-]+"
         ">--[-[<->+++[-]]]<[++++++++++++<[>-[>+>>]>[+[<+>-]>+>>]<<<<<-]>>[<+>-"
         "]>[-[-<<[-]>>]<<[<<->>-]>>]<<[<<+>>-]]<[-]<.[-]<-,+]"},
    {.name = "FizzBuzz", // https://codereview.stackexchange.com/a/104022,
     .program =
         "++++++++++[>++++++++++<-]>>++++++++++>->>>>>>>>>>-->+++++++[->+++++++"
         "+++<]>[->+>+>+>+<<<<]+++>>+++>>>++++++++[-<++++<++++<++++>>>]+++++[-<"
         "++++<++++>>]>-->++++++[->+++++++++++<]>[->+>+>+>+<<<<]+++++>>+>++++++"
         ">++++++>++++++++[-<++++<++++<++++>>>]++++++[-<+++<+++<+++>>>]>-->---+"
         "[-<+]-<<[->>>+>++[-->++]-->+++[---<-->+>-[<<++[>]]>++[--+[-<+]->>[-]+"
         "++++[---->++++]-->[->+<]>>[.>]>++]-->+++]---+[-<+]->>-[+>++++++++++<<"
         "[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]>>[-]>>>++++++++++<[->->+<<]>[-]>[<+"
         "+++++[->++++++++<]>.[-]]<<++++++[-<++++++++>]<.[-]<<[-<+>]]<<<.<]"},
    {.name = "sierpinski", // http://brainfuck.org/sierpinski.b
     .program =
         "++++++++[>+>++++<<-]>++>>+<[-[>>+<<-]+>>]>+[-<<<[->[+[-]+>++>>>-<"
         "<]<[<]>>++++++[<<+++++>>-]+<<++.[-]<<]>.>+[>>]>+]"},
    {.name = "quine", // http://www.brainfuck.org/392quine.b
     .program =
         "->++>+++>+>+>+++>>>>>>>>>>>>>>>>>>>>+>+>++>+++>++>>+++>+>>>>>>>>>>>>>"
         ">>>>>>>>>>>>>>>>>>>>+>+>>+++>>+++>>>>>+++>+>>>>>>>>>++>+++>+++>+>>+++"
         ">>>+++>+>++>+++>>>+>+>++>+++>+>+>>+++>>>>>>>+>+>>>+>+>++>+++>+++>+>>+"
         "++>>>+++>+>++>+++>++>>+>+>++>+++>+>+>>+++>>>>>+++>+>>>>>++>+++>+++>+>"
         ">+++>>>+++>+>+++>+>>+++>>+++>>++[[>>+[>]++>++[<]<-]>+[>]<+<+++[<]<+]>"
         "+[>]++++>++[[<++++++++++++++++>-]<+++++++++.<]"},
    {}, // NULL terminator
};

// better in .data than in the stack
char brainfuck_tape[BRAINFUCK_TAPE_SIZE] = {0};

/// @brief Brainfuck interpreter
void brainfuck(uint8_t screen_nbr, const char *input) {
  vga_info screen_info = {
      .screen = screen_nbr, .nowrapchar = true, .print = true};
  const char *ip = input;    /// instruction pointer
  char *dp = brainfuck_tape; /// data pointer
  size_t level;              /// how deep inside loops we are

  bzero(dp, sizeof(brainfuck_tape));
  for (; *ip; ++ip) {
    switch (*ip) {
    case '>':
      ++dp;
      break;
    case '<':
      --dp;
      break;
    case '+':
      ++(*dp);
      break;
    case '-':
      --(*dp);
      break;
    case '.':
      vga_printf(screen_info, "%c", *dp);
      break;
    case ',':
      *dp = get_char();
      break;
    case '[': // jump to next bracket if *dp == 0
      if (!*dp) {
        for (level = 1; level;) {
          ++ip; // needs to be done first
          if (*ip == '[')
            ++level;
          else if (*ip == ']')
            --level;
        }
      }
      break;
    case ']': // jump to previous bracket if *dp != 0
      if (*dp) {
        for (level = 1; level;) {
          ip--; // needs to be done first
          if (*ip == '[')
            --level;
          if (*ip == ']')
            ++level;
        }
        --ip;
      }
      break;

    default:
      break;
    }
  }
}

int brainfuck_demo_mode(uint8_t screen_nbr) {
  vga_info screen_info = {
      .screen = screen_nbr, .nowrapchar = true, .print = true};
  char input[3] = {0};
  int i, input_nbr;
  struct brainfuck_demo_tape choice;

  // program list
  vga_printf(screen_info, "No input provided, running in demo mode instead:\n");
  for (i = 0; *brainfuck_demo_tapes[i].name; ++i)
    vga_printf(screen_info, "\t%u) %s\n", i, brainfuck_demo_tapes[i].name);

  // prompt input choser
  vga_printf(screen_info, "Program(nbr)> ");
  if (!get_line(input, screen_nbr, sizeof(input) - sizeof(char))) { //-1 to \0
    return 1;
  }
  input_nbr = atoi(input);
  if (input_nbr < 0 || input_nbr >= i) {
    vga_printf(screen_info, "Error: Selection out of range");
    return 1;
  }

  choice = brainfuck_demo_tapes[input_nbr];
  vga_printf(screen_info, "Executing \"%s\" (%u bytes):\n%s\n", choice.name,
             strlen(choice.program), choice.program);
  brainfuck(screen_nbr, choice.program);
  return 0;
}

int cmd_brainfuck(uint8_t screen_nbr, int ac, char **av) {
  if (!ac) {
    return brainfuck_demo_mode(screen_nbr);
  }

  brainfuck(screen_nbr, av[0]);
  return 0;
}
