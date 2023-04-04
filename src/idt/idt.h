#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_NB_ENTRIES 256

struct interrupt_desc {
  uint16_t base_low; /// address of the ISR (interrupt service routine) to jump
                     /// to when this interrupt is called
  uint16_t segment_selector; /// code segment selector : where the interrupts
                             /// handler reside
  uint8_t _reserved;         /// set to 0
  uint8_t access;
  uint16_t base_high; /// address is divided in two parts
} __attribute__((packed));

struct idt_ptr {
  uint16_t limit; /// size of idt
  uint32_t base;  /// address
} __attribute__((packed));

typedef struct int_frame {
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t sp;
  uint32_t ss;
} int_frame __attribute__((packed));

// API

/// @brief init idt_ptr, fill idt_entry, load idt in idt register
void init_idt();

// Interrupt handlers
void interrupt_exception_handler();

#endif
