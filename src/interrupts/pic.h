#ifndef IRQ_H
#define IRQ_H

#include <stdbool.h>
#include <stdint.h>

/// master PIC
#define PIC1 0x20
#define PIC1_DATA PIC1 + 1

/// slave PIC
#define PIC2 0xA0
#define PIC2_DATA PIC1 + 1

/// PIC ///

// See the datasheet
// https://pdos.csail.mit.edu/6.828/2017/readings/hardware/8259A.pdf

/// @brief First Initialization Command Word, the PIC will then wait for 2 or 3
/// more words depending on the value of the `icw4_needed` register
typedef union {
  struct __attribute__((packed)) icw1_s {
    bool icw4_needed : 1;          /// if==0: no ICW4 needed (!set to true!)
    bool single_mode : 1;          /// if==0: cascade mode
    bool interval_4 : 1;           /// if==0: interval 8
    bool level_triggered_mode : 1; /// if==0: edge triggered mode
    bool enable : 1;               /// !!SET THIS TO TRUE!!
    uint8_t mcs_only : 3;          /// mcs-80 mode only int vect addr
  } data;
  uint8_t byte; /// cast to byte
} icw1_t;

/// @brief last InitializationCommandWord, only if `icw4_needed` is set in ICW1
typedef union {
  struct __attribute__((packed)) icw4_s {
    bool mode_8086 : 1;       /// if==0: MCS-80 mode (!set to true !)
    bool auto_eoi : 1;        /// if==0: Normal EOI
    bool buffered_master : 1; /// if==0 && buffered_mode==1: buffered mode slave
    bool buffered_mode : 1;   /// if==0: non buffered mode
    bool special_fully_nested_mode : 1; /// if==0: non __
  } data;
  uint8_t byte;
} icw4_t;

// Control functions

/// @brief Initialize the PIC in master/slave & 8086 mode and mask their IRQs
/// @param master_vector_offset offsets to map master IRQ word at in the IDT
/// @param slave_vector_offset offsets to map slave IRQ word at in the IDT
void init_pic(uint8_t master_vector_offset, uint8_t slave_vector_offset);

#endif
