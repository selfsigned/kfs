#ifndef IDT_H
#define IDT_H

#include "interrupts.h"

#include <stdbool.h>
#include <stdint.h>

#ifndef SCREEN_ERROR
#define SCREEN_ERROR 9
#endif

#define IDT_NB_ENTRIES 256

//// IDT ////

/// @brief IDT gates and ranges
enum idt_gates {

  EXCEPT_DIV0 = 0,
  EXCEPT_DOUBLE_FAULT = 8,
  EXCEPT_INVALID_TSS = 10,
  EXCEPT_PAGE_FAULT = 14,
  EXCEPT_ALIGNMENT_CHECK = 17,
  EXCEPT_CTRL_PROTECTION = 21,
  EXCEPT_RESERVED = 31,

  DEVICE_START = 32,
  DEVICE_END = 127,

  SYSCALL = 128,

  OTHER = 129,
  END = 255
};

enum idt_access {
  TRAP_GATE_FLAGS =
      0x8F, /// P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
  INT_GATE_FLAGS =
      0x8E, // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
};

/// @brief entry in the IDT
struct idt_interrupt_desc {
  uint16_t base_low;          /// address of the ISR to call (LOW bytes)
  uint16_t segment_selector;  /// code segment selector : where the
                              /// interrupts handler reside
  uint8_t _reserved;          /// set to 0
  enum idt_access access : 8; /// interrupt type and permissions
  uint16_t base_high;         ///  addres of the ISR to call (HIGH bytes)
} __attribute__((packed));

/// @brief pointer to be loaded in the IDT
struct idt_ptr {
  uint16_t limit; /// size of idt
  uint32_t base;  /// address
} __attribute__((packed));

//// ISR ////

/// @brief pre-interrupt frame, argument to ISRs
typedef struct __attribute__((packed)) int_frame {
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t sp;
  uint32_t ss;
} int_frame;

// API

/// @brief initialize the idt and load it
void init_idt();

#endif
