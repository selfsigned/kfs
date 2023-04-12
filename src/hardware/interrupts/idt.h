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

// offset to map the PIC at
#define IDT_PIC_OFFSET 32             /// start of user defined interrupts
#define IDT_SYSCALL 0x80              /// address of the syscall handler
#define IDT_SW_OFFSET IDT_SYSCALL + 1 /// misc interrupts

typedef enum idt_access {
  INT_GATE_FLAGS =
      0x8E, // P = 1, DPL = 00, S = 0, Type = 1110 (32bit interrupt gate)
  TRAP_GATE_FLAGS =
      0x8F, /// P = 1, DPL = 00, S = 0, Type = 1111 (32bit trap gate)
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

/// @brief add an idt entry
/// @param entry_nbr ISR number to give to the function
/// @param int_handler Interrupt handler
/// @param access Access level (set RING permissions and if TRAP or INT)
void idt_add_entry(uint8_t entry_nbr, void (*int_handler)(int_frame *frame),
                   idt_access access);

/// @brief delete an IDT entry
/// @param entry_nbr IDT entry to remove
void idt_del_entry(uint8_t entry_nbr);

/// @brief get an idt entry
struct idt_gate_desc idt_get_entry(uint8_t entry_nbr);

#endif
