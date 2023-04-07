#include "../drivers/vga/vga.h"
#include "idt.h"

/// @brief print exception
/// @param frame stack frame
/// @param msg message to print
/// @param code -1 if no code, otherwise code
static void _print_exception(int_frame *frame, unsigned char *msg, int code) {
  vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
             "\n%a%S\n"
             "eip:%#.8x "
             "cs:%#.8x "
             "eflags:%#.8x "
             "sp:%#.8x "
             "ss:%#.8x\n",
             (vga_attributes){.fg = VGA_COLOR_LIGHT_RED}, msg, frame->eip,
             frame->cs, frame->eflags, frame->sp, frame->ss);
  if (code != -1)
    vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true}, "code: %d",
               code);
}

INTERRUPT void int_exception_div0(int_frame *frame) {
  _print_exception(frame, "DIV BY 0 EXCEPTION HANDLED", -1);

  frame->eip++; // go to next instruction
  return;
}

INTERRUPT void int_exception(int_frame *frame) {
  _print_exception(frame, "EXCEPTION RECEIVED, HALTING", -1);

  __asm__("hlt");
}

INTERRUPT void int_exception_code(int_frame *frame, uint32_t code) {
  _print_exception(frame, "EXCEPTION WITH CODE RECEIVED, HALTING", (int)code);

  __asm__("hlt");
}
