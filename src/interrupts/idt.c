#include "idt.h"
#include "../klibc/libc.h"

struct idt_interrupt_desc idt[IDT_NB_ENTRIES];
struct idt_ptr idt_ptr;

/// set to code segment (where interrupts handler reside)
#define IDT_CODE_SEGMENT_SELECTOR 0x08

#define IDT_ADD_FUNC(x)                                                        \
  .base_low = (x & 0xFFFF), .base_high = ((x >> 16) & 0xFFFF)

#define IDT_EXCEPTION_ISR                                                      \
  {                                                                            \
    IDT_ADD_FUNC((int)(&int_exception)),                                       \
        .access = INT_GATE_FLAGS,                                              \
        .segment_selector = IDT_CODE_SEGMENT_SELECTOR                          \
  }
#define IDT_EXCEPTION_CODE_ISR                                                 \
  {                                                                            \
    IDT_ADD_FUNC((int)(&int_exception_code)),                                  \
        .access = INT_GATE_FLAGS,                                              \
        .segment_selector = IDT_CODE_SEGMENT_SELECTOR                          \
  }

void init_idt() {
  struct idt_interrupt_desc new_idt[IDT_NB_ENTRIES] = {
      // TODO explicitely init each interrupt instead of double def
      [EXCEPT_DIV0] = {IDT_ADD_FUNC((int)(&int_exception_div0)),
                       .access = TRAP_GATE_FLAGS,
                       .segment_selector = IDT_CODE_SEGMENT_SELECTOR},
      [EXCEPT_DIV0 + 1 ... EXCEPT_RESERVED] = IDT_EXCEPTION_ISR,
      [EXCEPT_DOUBLE_FAULT] = IDT_EXCEPTION_CODE_ISR,
      [EXCEPT_INVALID_TSS... EXCEPT_PAGE_FAULT] = IDT_EXCEPTION_CODE_ISR,
      [EXCEPT_ALIGNMENT_CHECK] = IDT_EXCEPTION_CODE_ISR,
      [EXCEPT_CTRL_PROTECTION] = IDT_EXCEPTION_CODE_ISR,
  };
  memcpy(idt, new_idt, sizeof(idt));

  idt_ptr.limit = (sizeof(struct idt_interrupt_desc) * IDT_NB_ENTRIES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  __asm__("lidt %0" ::"memory"(idt_ptr)); // L(oad)IDT
}
