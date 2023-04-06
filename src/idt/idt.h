#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_NB_ENTRIES 256

#define TRAP_GATE_FLAGS                                                        \
  0x8F // P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
#define INT_GATE_FLAGS                                                         \
  0x8E // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)

struct interrupt_desc {
  uint16_t base_low; /// address of the ISR (interrupt service routine) to
                     /// jump to when this interrupt is called
  uint16_t segment_selector; /// code segment selector : where the
                             /// interrupts handler reside
  uint8_t _reserved;         /// set to 0
  uint8_t access;
  uint16_t base_high; /// address is divided in two parts
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit; /// size of idt
  uint32_t base;  /// address
} __attribute__((packed));

typedef struct __attribute__((packed)) int_frame {
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t sp;
  uint32_t ss;
} int_frame;

// API

/// @brief init idt_ptr, fill idt_entry, load idt in idt register
void init_idt();

/// Set attributes needed to iret and save the stack
#define INTERRUPT                                                              \
  __attribute__((target("general-regs-only"))) __attribute__((interrupt))

// Interrupt handlers
INTERRUPT void interrupt_exception_handler(int_frame *frame);

#endif
