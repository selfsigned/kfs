#include "shell.h"
#include "../klibc/libc.h"
#include "cmds/cmd_greet.h"

shell_cmd *shell_get_cmd(char *name) {
  for (struct shell_cmd *cmd = g_shell_cmds; !(cmd->flags & SHELL_CMD_END);
       cmd++)
    if (!strncmp(cmd->name, name, SHELL_MAX_CMDNAME))
      return cmd;
  return NULL;
}

void shell_run(shell_state *state, struct kbd_state key) {
  vga_info screen_info = {.screen = state->screen};

  if (!state->greeted) {
    cmd_greet(state->screen, 0, NULL);
    state->greeted = true;
    goto next_line;
  }

  // add a character
  if (isprint(key.ascii)) {
    if (state->input.idx < SHELL_MAX_INPUT) {
      state->input.buf[state->input.idx++] = key.ascii;
      vga_printf(screen_info, "%c", key.ascii);
    }
    // TODO scroll_state = 0;
  } else {
    switch (key.key) {

    // remove a character
    case KBD_BACKSPACE: {
      if (state->input.idx) {
        state->input.buf[--state->input.idx] = '\0';
        vga_printf(screen_info, "%c", '\b');
      }
      break;
    }
    // The meat of the shell, execute a command
    case KBD_ENTER: {
      char *arg_cmd = NULL;
      char *argv[SHELL_MAX_ARGV] = {};
      size_t argc = 0;

      if (!state->input.buf[0] || !(arg_cmd = strtok(state->input.buf, " ")))
        goto next_line;

      for (char *ptr;
           (ptr = strtok(NULL, " ")) != NULL && argc < SHELL_MAX_ARGV; argc++) {
        argv[argc] = ptr;
      }

      struct shell_cmd *cmd = NULL;
      if ((cmd = shell_get_cmd(arg_cmd))) {
        vga_printf(screen_info, "\n");
        state->return_code = cmd->func(state->screen, argc, argv);
        state->last_cmd = cmd;
      } else {
        vga_printf(screen_info, "\nCommand not found: \"%s\"", arg_cmd);
      }

    next_line:
      vga_printf( // reset colors, go to newline, print PS1
          screen_info, "%a\n" SHELL_PS1,
          (vga_attributes){.bg = VGA_COLOR_BLACK, .fg = VGA_COLOR_WHITE});
      bzero(state->input.buf, sizeof(state->input.buf));
      state->input.idx = 0;
      break;
    }

    default:
      break;
    }
  }
}
