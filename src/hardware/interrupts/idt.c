#include "idt.h"
#include "../../drivers/keyboard/keyboard.h"
#include "../../drivers/keyboard/ps2.h"
#include "../../drivers/vga/vga.h"
#include "../../kernel.h"
#include "../../klibc/libc.h"
#include "../../log.h"

struct idt_gate_desc idt[IDT_NB_GATES];
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
             "eip:%p "
             "cs:%p "
             "eflags:%p"
             "\n\t\t"
             "sp: %p "
             "ss:%p",
             msg, frame->eip, frame->cs, frame->eflags, frame->sp, frame->ss);
  if (has_code)
    vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
               "\n\t\tcode:%#.8b", code);
}

/// @brief Do what must be done after an exception has been raised
static void _handle_exception() {
  vga_printf((vga_info){.screen = 9, .print = true}, "\n\n\n%52s",
             "Press any key to restart");
  kbd_poll(); // wait on kbd
  kbd_poll(); // debounce (no pressed/release management)
  outb(KBD_IO_COMMAND_REGISTER,
       KBD_IO_SHUTDOWN);   // restart using the PS/2 controller
  __asm__ volatile("hlt"); // halt the cpu until shutdown
}

/// @brief handle div by 0 exceptions
/// @param frame stack frame
INTERRUPT void _exception_div0(int_frame *frame) {
  WARN_MSG("Div by 0 exception handled\n\t->eip:%p cs:%p eflags:%p sp:%p ss:%p",
           frame->eip, frame->cs, frame->eflags, frame->sp, frame->ss);

  frame->eip++; // go to next instruction
  return;
}

#define INT_EXCEPTION_HANDLER(N, MSG)                                          \
  INTERRUPT void _exception_##N(int_frame *frame) {                            \
    __asm__ volatile("cli");                                                   \
    _print_exception(frame, "EXCEPTION #" #N ": " #MSG, false, 0);             \
    _handle_exception();                                                       \
    __asm__ volatile("sti");                                                   \
  }

#define INT_EXCEPTION_HANDLER_CODE(N, MSG)                                     \
  INTERRUPT void _exception_##N(int_frame *frame, uint32_t code) {             \
    __asm__ volatile("cli");                                                   \
    _print_exception(frame, "EXCEPTION #" #N ": " #MSG, true, code);           \
    _handle_exception();                                                       \
    __asm__ volatile("sti");                                                   \
  }

// generic exceptions
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
  [x] = {IDT_ADD_FUNC((int)(&_exception_##x)), .access = TRAP_GATE_FLAGS,      \
         .segment_selector = IDT_CODE_SEGMENT_SELECTOR}

void idt_init() {
  struct idt_gate_desc new_idt[IDT_NB_GATES] = {
      [0] = {IDT_ADD_FUNC((uint32_t)(&_exception_div0)),
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

  idt_ptr.limit = (sizeof(struct idt_gate_desc) * IDT_NB_GATES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  __asm__ volatile("lidt %0" ::"memory"(idt_ptr)); // L(oad)IDT
}

struct idt_gate_desc idt_get_gate(uint8_t gate_nbr) {
  // returns the idt entry (clang-format doesn't like inline returns)
  return idt[gate_nbr];
}

void idt_add_gate(uint8_t gate_nbr, void (*int_handler)(int_frame *frame),
                  idt_access access) {
  idt_gate_desc *descriptor = &idt[gate_nbr];

  descriptor->base_low = ((uint32_t)int_handler & 0xFFFF);
  descriptor->base_high = (((uint32_t)int_handler >> 16) & 0xFFFF);
  descriptor->access = access;
  descriptor->segment_selector = IDT_CODE_SEGMENT_SELECTOR;
  DEBUG_MSG("IDT gate %d set (isr:%p,access:%#.8b)", gate_nbr, int_handler,
            access);
}

void idt_del_gate(uint8_t gate_nbr) {
  bzero(&idt[gate_nbr], sizeof(idt_gate_desc));
  DEBUG_MSG("Unset IDT gate:%xh", gate_nbr);
}
