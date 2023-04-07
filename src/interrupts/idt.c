#include "idt.h"
#include "../drivers/vga/vga.h"
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
  vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
             "\n%a%s\n"
             "eip:%#.8x "
             "cs:%#.8x "
             "eflags:%#.8x "
             "sp:%#.8x "
             "ss:%#.8x",
             (vga_attributes){.fg = VGA_COLOR_LIGHT_RED}, msg, frame->eip,
             frame->cs, frame->eflags, frame->sp, frame->ss);
  if (has_code)
    vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true}, " code: %u",
               code);
}

/// @brief handle div by 0 exceptions
/// @param frame stack frame
INTERRUPT void _exception_div0(int_frame *frame) {
  _print_exception(frame, "DIV BY 0 EXCEPTION HANDLED", false, 0);

  frame->eip++; // go to next instruction
  return;
}

#define INT_EXCEPTION_HANDLER(name)                                            \
  INTERRUPT void name(int_frame *frame) {                                      \
    _print_exception(frame, "RECEIVED INT " #name ", HALTING", false, 0);      \
    __asm__("hlt");                                                            \
  }

#define INT_EXCEPTION_HANDLER_CODE(name)                                       \
  INTERRUPT void name(int_frame *frame, uint32_t code) {                       \
    _print_exception(frame, "RECEIVED INT " #name ", HALTING", true, code);    \
    __asm__("hlt");                                                            \
  }

// generic exceptions, let's not talk about it
INT_EXCEPTION_HANDLER(_exception_1)
INT_EXCEPTION_HANDLER(_exception_2)
INT_EXCEPTION_HANDLER(_exception_3)
INT_EXCEPTION_HANDLER(_exception_4)
INT_EXCEPTION_HANDLER(_exception_5)
INT_EXCEPTION_HANDLER(_exception_6)
INT_EXCEPTION_HANDLER(_exception_7)
INT_EXCEPTION_HANDLER_CODE(_exception_8)
INT_EXCEPTION_HANDLER(_exception_9)
INT_EXCEPTION_HANDLER_CODE(_exception_10)
INT_EXCEPTION_HANDLER(_exception_11)
INT_EXCEPTION_HANDLER(_exception_12)
INT_EXCEPTION_HANDLER(_exception_13)
INT_EXCEPTION_HANDLER_CODE(_exception_14)
INT_EXCEPTION_HANDLER_CODE(_exception_15)
INT_EXCEPTION_HANDLER_CODE(_exception_16)
INT_EXCEPTION_HANDLER_CODE(_exception_17)
INT_EXCEPTION_HANDLER(_exception_18)
INT_EXCEPTION_HANDLER(_exception_19)
INT_EXCEPTION_HANDLER(_exception_20)
INT_EXCEPTION_HANDLER_CODE(_exception_21)
INT_EXCEPTION_HANDLER(_exception_22)
INT_EXCEPTION_HANDLER(_exception_23)
INT_EXCEPTION_HANDLER(_exception_24)
INT_EXCEPTION_HANDLER(_exception_25)
INT_EXCEPTION_HANDLER(_exception_26)
INT_EXCEPTION_HANDLER(_exception_27)
INT_EXCEPTION_HANDLER(_exception_28)
INT_EXCEPTION_HANDLER(_exception_29)
INT_EXCEPTION_HANDLER(_exception_30)
INT_EXCEPTION_HANDLER(_exception_31)

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
