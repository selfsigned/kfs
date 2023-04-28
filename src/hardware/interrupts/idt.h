#ifndef IDT_H
#define IDT_H

#include "interrupts.h"

#include "../../log.h"
#include <stdbool.h>
#include <stdint.h>

#define IDT_NB_GATES 256

//// IDT/IVT ////
// we'll referer to both as the IDT for simplicity's sake

// offset to map the PIC at
#define IDT_PIC_OFFSET 32             /// start of user defined interrupts
#define IDT_SYSCALL 0x80              /// address of the syscall handler
#define IDT_SW_OFFSET IDT_SYSCALL + 1 /// misc interrupts

typedef enum idt_access {
  INT_GATE_FLAGS =
      0x8E, // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
  TRAP_GATE_FLAGS =
      0x8F, /// P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
  INT_GATE_USER_FLAGS =
      0xEE, /// P = 1, DPL = 11, S = 0, Type = 1110 (RING(PL)3)
} idt_access;

/// @brief entry in the IDT
typedef struct __attribute__((packed)) idt_gate_desc {
  uint16_t base_low;          /// address of the ISR to call (LOW bytes)
  uint16_t segment_selector;  /// code segment selector : where the
                              /// interrupts handler reside
  uint8_t _reserved;          /// set to 0
  enum idt_access access : 8; /// interrupt type and permissions
  uint16_t base_high;         ///  addres of the ISR to call (HIGH bytes)
} idt_gate_desc;

/// @brief pointer to be loaded in the IDT
struct idt_ptr {
  uint16_t limit; /// size of idt
  uint32_t base;  /// address
} __attribute__((packed));

// API

/// @brief initialize the idt and load it
void idt_init();

/// @brief set an IDT gate
/// @param gate_nbr gate to edit
/// @param int_handler Interrupt handler
/// @param access Access level (set RING permissions and if TRAP or INT)
void idt_add_gate(uint8_t gate_nbr, void (*int_handler)(int_frame *frame),
                  idt_access access);

/// @brief unset an IDT gate
/// @param entry_nbr IDT entry to remove
void idt_del_gate(uint8_t gate_nbr);

/// @brief get an idt gate
struct idt_gate_desc idt_get_gate(uint8_t gate_nbr);

#endif
