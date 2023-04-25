#include "cmd_stack.h"
#include "../../cp437.h"
#include "../../klibc/get_line.h"
#include "../../klibc/libc.h"

struct stack_frame {
  struct stack_frame *ebp;
  void *ret;
} __attribute__((packed));

uint8_t g_stack_frame_screen = 0;

void _stack_printframes(struct stack_frame *frame, size_t count) {
  vga_printf((vga_info){.screen = g_stack_frame_screen, .print = true},
             "%d: frame->ret:%p frame->ebp:%p size:%u\n", count, frame->ret,
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

/// @brief equivalent of hexdump -C, doesn't care about endian-ness
/// @param screen_nbr screen to print to
/// @param data what to dump
/// @param size how many bytes to dump
void _hexdump(uint8_t screen_nbr, void *data, size_t size) {
  vga_info screen_info = {.screen = screen_nbr};
  const uint8_t *p = (const uint8_t *)data;
  for (size_t i = 0; i < size; i += 16) {
    vga_printf(screen_info, "%10p: ", (uint32_t)data + i);
    for (size_t j = 0; j < 16; j++) {
      if (i + j < size) {
        vga_printf(screen_info, "%02X ", p[i + j]);
      } else {
        vga_printf(screen_info, "   ");
      }
    }
    vga_printf(screen_info, "%C", CP437_BOX_DRAWINGS_LIGHT_VERTICAL);
    for (size_t j = 0; j < 16; j++) {
      if (i + j < size) {
        uint8_t c = p[i + j];
        if (c >= 32 && c <= 126) {
          vga_printf(screen_info, "%c", c);
        } else {
          vga_printf(screen_info, ".");
        }
      } else {
        vga_printf(screen_info, " ");
      }
    }
    vga_printf(screen_info, "%C\n", CP437_BOX_DRAWINGS_LIGHT_VERTICAL);
  }
}

void _stack_dumpframe(struct stack_frame *frame, size_t count) {
  if (count == g_chosen_frame)
    _hexdump(g_stack_frame_screen, (void *)(frame->ret),
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
