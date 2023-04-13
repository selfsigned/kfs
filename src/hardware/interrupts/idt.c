#include "idt.h"
#include "../../drivers/keyboard/keyboard.h"
#include "../../drivers/vga/vga.h"
#include "../../kernel.h"
#include "../../klibc/libc.h"

struct idt_gate_desc idt[IDT_NB_ENTRIES];
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

/// @brief Do what must be done after an exception has been raised
static void _handle_exception() {
  vga_printf((vga_info){.screen = 9, .print = true}, "\n\n\n%52s",
             "Press any key to restart");
  while (!kbd_get()) { // wait on keyboard
  }
  io_wait();
  while (!kbd_get()) { // debounce x2
  }
  outb(0x64, 0xFE); // restart using the PS/2 controller
  __asm__("hlt");   // rest
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
    __asm__("cli");                                                            \
    _print_exception(frame, "EXCEPTION #" #N ": " #MSG, false, 0);             \
    _handle_exception();                                                       \
    __asm__("sti");                                                            \
  }

#define INT_EXCEPTION_HANDLER_CODE(N, MSG)                                     \
  INTERRUPT void _exception_##N(int_frame *frame, uint32_t code) {             \
    __asm__("cli");                                                            \
    _print_exception(frame, "EXCEPTION #" #N ": " #MSG, true, code);           \
    _handle_exception();                                                       \
    __asm__("sti");                                                            \
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

////Syscalls////

#define NB_HANDLED_SYSCALL 2

///@brief function syscall number one (need 2 to test de dispatcher)
void syscall_1() {
  vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
             "Sycall 1 called\n");
}

///@brief function syscall number two (need 2 to test de dispatcher)
void syscall_2() {
  vga_printf((vga_info){.screen = SCREEN_ERROR, .print = true},
             "Sycall 2 called\n");
}

// array function pointer to pass au syscall, so the nb of the syscall place in
// eax will match to the correspondin function
void *syscall_table[NB_HANDLED_SYSCALL] = {&syscall_1, &syscall_2};

/// @brief if handled syscall save registers before calling offset of
/// syscall_table function contained in eax reg
__attribute__((naked)) void syscall_dispatcher(void) {
  __asm__ __volatile__(
      ".intel_syntax noprefix\n"

      ".equ NB_HANDLED_SYSCALL, 2\n" // Check if handled syscall
      "cmp eax, NB_HANDLED_SYSCALL-1\n"
      "ja non_handled_syscall\n"

      "push eax\n"
      "push gs\n"
      "push fs\n"
      "push es\n"
      "push ds\n"
      "push ebp\n"
      "push edi\n"
      "push esi\n"
      "push edx\n"
      "push ecx\n"
      "push ebx\n"
      "push esp\n"
      "call [syscall_table+eax*4]\n" // call offset of our syscall table
      "add esp, 4\n"
      "pop ebx\n"
      "pop ecx\n"
      "pop edx\n"
      "pop esi\n"
      "pop edi\n"
      "pop ebp\n"
      "pop ds\n"
      "pop es\n"
      "pop fs\n"
      "pop gs\n"
      "add esp, 4\n"
      "iretd\n"

      "non_handled_syscall:\n"
      "iretd\n"

      ".att_syntax");
}

////IDT////

/// set to code segment (where interrupts handler reside)
#define IDT_CODE_SEGMENT_SELECTOR 0x08

#define IDT_ADD_FUNC(x)                                                        \
  .base_low = (x & 0xFFFF), .base_high = ((x >> 16) & 0xFFFF)

#define IDT_EXCEPTION(x)                                                       \
  [x] = {IDT_ADD_FUNC((int)(&_exception_##x)), .access = TRAP_GATE_FLAGS,      \
         .segment_selector = IDT_CODE_SEGMENT_SELECTOR}

void idt_init() {
  struct idt_gate_desc new_idt[IDT_NB_ENTRIES] = {
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

      [128] = {IDT_ADD_FUNC((uint32_t)(&syscall_dispatcher)),
               .access = INT_GATE_FLAGS,
               .segment_selector = IDT_CODE_SEGMENT_SELECTOR}};
  memcpy(idt, new_idt, sizeof(idt));

  idt_ptr.limit = (sizeof(struct idt_gate_desc) * IDT_NB_ENTRIES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  __asm__("lidt %0" ::"memory"(idt_ptr)); // L(oad)IDT
}

struct idt_gate_desc idt_get_entry(uint8_t entry_nbr) {
  // returns the idt entry (clang-format doesn't like inline returns)
  return idt[entry_nbr];
}

void idt_add_entry(uint8_t entry_nbr, void (*int_handler)(int_frame *frame),
                   idt_access access) {
  idt_gate_desc *descriptor = &idt[entry_nbr];

  descriptor->base_low = ((uint32_t)int_handler & 0xFFFF);
  descriptor->base_high = (((uint32_t)int_handler >> 16) & 0xFFFF);
  descriptor->access = access;
  descriptor->segment_selector = IDT_CODE_SEGMENT_SELECTOR;
}

void idt_del_entry(uint8_t entry_nbr) {
  bzero(&idt[entry_nbr], sizeof(idt_gate_desc));
}
