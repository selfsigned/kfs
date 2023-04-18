#include "../drivers/keyboard/keyboard.h"
#include "../drivers/vga/vga.h"
#include "../klibc/libc.h"
#include "screens.h"

int fullhelp(uint8_t screen_nbr, int ac, char *av[]);

enum shell_cmd_flags {
  SHELL_CMD_END = 255,
};

#define SHELL_MAX_INPUT 1920
#define SHELL_MAX_CMDNAME 25
#define SHELL_MAX_ARGV 9

struct command {
  uint8_t flags;
  char name[SHELL_MAX_CMDNAME];
  int (*func)(uint8_t screen_nbr, int argc, char **argv);
  struct {
    char smsg[80];
    char lmsg[400];
  } help;
};

struct command g_cmds[] = {
    {0,
     "help",
     fullhelp,
     {"Get full help for a command", "Displays the entire help"}},
    {0,
     "nothelp",
     fullhelp,
     {"get no help", "seriously bro? didn't you see the name?"}},
    {.flags = SHELL_CMD_END},
};

// struct command *

struct command *shell_get_cmd(char *name) {
  for (struct command *cmd = g_cmds; !(cmd->flags & SHELL_CMD_END); cmd++)
    if (!strncmp(cmd->name, name, SHELL_MAX_CMDNAME))
      return cmd;
  return NULL;
}

int fullhelp(uint8_t screen_nbr, int ac, char *av[]) {
  vga_info screen_info = {.screen = screen_nbr};
  struct command *cmd = NULL;

  // display short help for every cmd if no params
  if (ac < 1) {
    vga_printf(screen_info, "Commands:\n");
    for (struct command *cmd = g_cmds; !(cmd->flags & SHELL_CMD_END); cmd++)
      vga_printf(screen_info, "%-10s%s\n", cmd->name, cmd->help.smsg);
    vga_printf(screen_info, "\nTo get detailed informations: help <cmd>");
    return 0;
  }

  // display short help and long help
  while (ac--) {
    if ((cmd = shell_get_cmd(av[ac]))) {
      vga_printf(screen_info, "%s - %s:\n%s\n", cmd->name, cmd->help.smsg,
                 cmd->help.lmsg);
    }
  }

  return 0;
}

static int set_scroll(uint8_t screen_nbr, kbd_scancode code, int scroll_state,
                      bool isnotup) {
  switch (code) {
  case KBD_HOME:
    scroll_state = -1;
    break;
  case KBD_END:
    scroll_state = 0;
    break;
  case KBD_CURSOR_UP:
    if (isnotup)
      scroll_state++;
    break;
  case KBD_CURSOR_DOWN:
    if (scroll_state > 0)
      scroll_state--;
    if (scroll_state == -1) {
      scroll_state = vga_screen_getscrolloffset(screen_nbr) - 1;
    }
    break;
  default:
  }
  return scroll_state;
}

void screen_loop(uint8_t home_screen, uint8_t note_screen) {
  uint8_t current_screen = home_screen;
  int scroll_state = 0;
  bool isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

  char input[SHELL_MAX_INPUT] = {};
  size_t input_index = 0;
  while (true) {
    if (kbd.status == KEY_RELEASED)
      goto halt;

    if ((isprint(kbd.ascii) || kbd.ascii == '\t') &&
        current_screen == note_screen) {
      if (input_index < SHELL_MAX_INPUT) {
        input[input_index++] = kbd.ascii;
        vga_printf((vga_info){.screen = note_screen}, "%c", kbd.ascii);
      }

      scroll_state = 0;
    } else {
      // control characters
      switch (kbd.key) {
      case KBD_BACKSPACE:
        if (input_index) {
          input[--input_index] = '\0';
          vga_printf((vga_info){.screen = note_screen}, "%c", '\b');
        }
        break;
      case KBD_F1 ... KBD_F10:
        // switch screen
        current_screen = kbd.key - KBD_F1;
        scroll_state = 0;
        break;
      case KBD_HOME ... KBD_PAGE_DOWN:
        // scroll
        scroll_state =
            set_scroll(current_screen, kbd.key, scroll_state, isnotup);
        break;
      case KBD_ENTER: {
        if (current_screen == note_screen) {
          char *arg_cmd;
          char *argv[SHELL_MAX_ARGV] = {};
          size_t argc = 0;

          if (!input[0] || !(arg_cmd = strtok(input, " ")))
            break;

          for (char *ptr;
               (ptr = strtok(NULL, " ")) != NULL && argc < SHELL_MAX_ARGV;
               argc++) {
            argv[argc] = ptr;
          }

          for (struct command *cmd = g_cmds; !(cmd->flags & SHELL_CMD_END);
               cmd++) {
            if (!strncmp(cmd->name, arg_cmd, SHELL_MAX_CMDNAME)) {
              vga_printf((vga_info){.screen = note_screen}, "\n");
              cmd->func(note_screen, argc, argv);
            }
          }

          vga_printf((vga_info){.screen = note_screen}, "\n>");
          bzero(input, sizeof(input));
          input_index = 0;
        }
        break;
      }
      default:
        break;
      }
    }

    if (current_screen == note_screen)
      vga_screen_setvgacursor(note_screen, true);
    isnotup = vga_screen_show_scrolled(current_screen, scroll_state);

  halt:
    __asm__ volatile("HLT"); // halt until next interrupt
  }
}
