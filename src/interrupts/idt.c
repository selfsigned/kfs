#include "idt.h"
#include "../drivers/vga/vga.h"
#include "../kernel.h"
#include "../klibc/libc.h"

struct idt_interrupt_desc idt[IDT_NB_ENTRIES];
struct idt_ptr idt_ptr;

////Exceptions////

/// @brief print exception
/// @param frame stack frame
/// @param msg message to print
/// @param code -1 if no code, otherwise code
static void _print_exception(int_frame *frame, char *msg, bool has_code,
                             uint32_t code) {
  vga_screen_clear(SCREEN_ERROR);
  vga_screen_fillbackground(SCREEN_ERROR, VGA_COLOR_BLUE);
  vga_printf((vga_info){.screen = SCREEN_ERROR, .column = 37, .row = 7},

             "%a %s %a\n\n\tAn error has occurred:\n",
             (vga_attributes){.bg = VGA_COLOR_LIGHT_GREY, .fg = VGA_COLOR_BLUE},
             OS_NAME,
             (vga_attributes){.bg = VGA_COLOR_BLUE, .fg = VGA_COLOR_WHITE});
  vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
             "\n\t%s\n\n\t\t"
             "eip:%#.8x "
             "cs:%#.8x "
             "eflags:%#.8x"
             "\n\t\t"
             "sp: %#.8x "
             "ss:%#.8x",
             msg, frame->eip, frame->cs, frame->eflags, frame->sp, frame->ss);
  if (has_code)
    vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
               "\n\t\tcode:%#.8x", code);
}

/// @brief handle div by 0 exceptions
/// @param frame stack frame
INTERRUPT void _exception_div0(int_frame *frame) {
  _print_exception(frame, "DIV BY 0 EXCEPTION HANDLED", false, 0);

  frame->eip++; // go to next instruction
  return;
}

#define INT_EXCEPTION_HANDLER(N, MSG)                                          \
  INTERRUPT void _exception_##N(int_frame *frame) {                            \
    _print_exception(frame, "EXCEPTION #" #N ": " #MSG, false, 0);             \
    __asm__("hlt");                                                            \
  }

#define INT_EXCEPTION_HANDLER_CODE(N, MSG)                                     \
  INTERRUPT void _exception_##N(int_frame *frame, uint32_t code) {             \
    _print_exception(frame, "EXCEPTION #" #N ": " #MSG, true, code);           \
    __asm__("hlt");                                                            \
  }

// generic exceptions, let's not talk about it
INT_EXCEPTION_HANDLER(1, RESERVED)
INT_EXCEPTION_HANDLER(2, NMI Interrupt)
INT_EXCEPTION_HANDLER(3, Breakpoint)
INT_EXCEPTION_HANDLER(4, Overflow)
INT_EXCEPTION_HANDLER(5, BOUND Range Exceeded)
INT_EXCEPTION_HANDLER(6, Invalid Opcode(Undefined Opcode))
INT_EXCEPTION_HANDLER(7, Device Not Available(No Math Coprocessor))
INT_EXCEPTION_HANDLER_CODE(8, Double Fault)
INT_EXCEPTION_HANDLER(9, Coprocessor Segment Overrun(Reserved))
INT_EXCEPTION_HANDLER_CODE(10, Invalid TSS)
INT_EXCEPTION_HANDLER(11, Segment Not Present)
INT_EXCEPTION_HANDLER(12, Stack - Segment Fault)
INT_EXCEPTION_HANDLER(13, General Protection)
INT_EXCEPTION_HANDLER_CODE(14, Page Fault)
INT_EXCEPTION_HANDLER_CODE(15, RESERVED)
INT_EXCEPTION_HANDLER_CODE(16, x87 FPU Floating - Point Error(Math Fault))
INT_EXCEPTION_HANDLER_CODE(17, Alignment Check)
INT_EXCEPTION_HANDLER(18, Machine Check)
INT_EXCEPTION_HANDLER(19, SIMD Floating - Point Exception)
INT_EXCEPTION_HANDLER(20, RESERVED)
INT_EXCEPTION_HANDLER_CODE(21, RESERVED)
INT_EXCEPTION_HANDLER(22, RESERVED)
INT_EXCEPTION_HANDLER(23, RESERVED)
INT_EXCEPTION_HANDLER(24, RESERVED)
INT_EXCEPTION_HANDLER(25, RESERVED)
INT_EXCEPTION_HANDLER(26, RESERVED)
INT_EXCEPTION_HANDLER(27, RESERVED)
INT_EXCEPTION_HANDLER(28, RESERVED)
INT_EXCEPTION_HANDLER(29, RESERVED)
INT_EXCEPTION_HANDLER(30, RESERVED)
INT_EXCEPTION_HANDLER(31, RESERVED)

////IDT////

/// set to code segment (where interrupts handler reside)
#define IDT_CODE_SEGMENT_SELECTOR 0x08

#define IDT_ADD_FUNC(x)                                                        \
  .base_low = (x & 0xFFFF), .base_high = ((x >> 16) & 0xFFFF)

#define IDT_EXCEPTION(x)                                                       \
  [x] = {IDT_ADD_FUNC((int)(&_exception_##x)), .access = INT_GATE_FLAGS,       \
         .segment_selector = IDT_CODE_SEGMENT_SELECTOR}

void init_idt() {
  struct idt_interrupt_desc new_idt[IDT_NB_ENTRIES] = {
      [EXCEPT_DIV0] = {IDT_ADD_FUNC((int)(&_exception_div0)),
                       .access = TRAP_GATE_FLAGS,
                       .segment_selector = IDT_CODE_SEGMENT_SELECTOR},
      // painful
      IDT_EXCEPTION(1),
      IDT_EXCEPTION(2),
      IDT_EXCEPTION(3),
      IDT_EXCEPTION(4),
      IDT_EXCEPTION(5),
      IDT_EXCEPTION(6),
      IDT_EXCEPTION(7),
      IDT_EXCEPTION(8),
      IDT_EXCEPTION(9),
      IDT_EXCEPTION(10),
      IDT_EXCEPTION(11),
      IDT_EXCEPTION(12),
      IDT_EXCEPTION(13),
      IDT_EXCEPTION(14),
      IDT_EXCEPTION(15),
      IDT_EXCEPTION(16),
      IDT_EXCEPTION(17),
      IDT_EXCEPTION(18),
      IDT_EXCEPTION(19),
      IDT_EXCEPTION(20),
      IDT_EXCEPTION(21),
      IDT_EXCEPTION(22),
      IDT_EXCEPTION(23),
      IDT_EXCEPTION(24),
      IDT_EXCEPTION(25),
      IDT_EXCEPTION(26),
      IDT_EXCEPTION(27),
      IDT_EXCEPTION(28),
      IDT_EXCEPTION(29),
      IDT_EXCEPTION(30),
      IDT_EXCEPTION(31),
  };
  memcpy(idt, new_idt, sizeof(idt));

  idt_ptr.limit = (sizeof(struct idt_interrupt_desc) * IDT_NB_ENTRIES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  __asm__("lidt %0" ::"memory"(idt_ptr)); // L(oad)IDT

  // Disable interrupts for now
  // see: https://wiki.osdev.org/Exceptions#Double_Fault
  // __asm__("sti"); // S(e)TI(nterrupt)
  __asm__("cli");
}
