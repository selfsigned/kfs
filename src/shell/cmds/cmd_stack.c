#include "cmd_stack.h"
#include "../../cp437.h"
#include "../../klibc/get_line.h"
#include "../../klibc/libc.h"
#include "cmd_hexdump.h"

struct stack_frame {
  struct stack_frame *ebp;
  void *ret;
} __attribute__((packed));

uint8_t g_stack_frame_screen = 0;

void _stack_printframes(struct stack_frame *frame, size_t count) {
  vga_printf((vga_info){.screen = g_stack_frame_screen, .print = true},
             "%d) frame->ret:%p frame->ebp:%p size:%u\n", count, frame->ret,
             frame->ebp, (void *)frame->ebp - (void *)frame->ret);
}

size_t _stack_walk(struct stack_frame *ebp, size_t max,
                   void (*callback)(struct stack_frame *, size_t)) {
  struct stack_frame *frame = ebp;
  size_t count;

  for (count = 0; frame && frame->ebp && count < max; ++count) {
    callback(frame, count);
    frame = frame->ebp;
  }
  return count;
}

size_t g_chosen_frame = 0;

void _stack_dumpframe(struct stack_frame *frame, size_t count) {
  if (count == g_chosen_frame)
    hexdump(g_stack_frame_screen, frame->ret,
            ((void *)frame->ebp - (void *)frame->ret));
}

void _get_ebp(struct stack_frame **ebp) {
  __asm__ volatile("mov %%ebp, %0" : "=r"(*ebp));
}

int cmd_stack(uint8_t screen_nbr, int ac, char **av) {
  vga_info screen_info = {.screen = screen_nbr, .print = true};
  int frame_max = (ac >= 1) ? atoi(av[0]) : STACKFRAME_NBR_DEFAULT;
  struct stack_frame *ebp; // stack frame to inspect
  size_t frame_nbr;        // nbr of frames found
  char input[4] = {0};     // user input buffer
  int input_nbr;           // user selected frame

  if (frame_max < 1) {
    vga_printf(screen_info, "Error: Number of frames must be at least 1");
    return 1;
  }

  // used to get cmd_stack in the frame list
  _get_ebp(&ebp);

  // show the stack frames
  g_stack_frame_screen = screen_nbr;
  frame_nbr = _stack_walk(ebp, frame_max, _stack_printframes);

  // Prompt the user for the stack frame
  vga_printf(screen_info, "Select frame%C ", CP437_RIGHT_ARROW);
  if (!get_line(input, screen_nbr, sizeof(input) - sizeof(char))) { //-1 to \0
    vga_printf(screen_info, "Error: Enter a number!");
    return 1;
  }
  input_nbr = atoi(input);
  if (input_nbr < 0) {
    vga_printf(screen_info, "Error: Selection has to be positive");
    return 1;
  } else if ((size_t)input_nbr >= frame_nbr) {
    vga_printf(screen_info, "Error: Selected frame doesn't exist");
    return 1;
  } else {
    g_chosen_frame = (size_t)input_nbr;
  }

  // Dump the chosen frame
  _stack_walk(ebp, g_chosen_frame + 1, _stack_dumpframe);
  return 0;
}
