#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

/// @brief Standard ISA IRQs
typedef enum irq_hw {
  IRQ_SYSTEM_TIMER,
  IRQ_PS2_KEYBOARD,
  IRQ_CASCADED,
  IRQ_COM2,
  IRQ_COM1,
  IRQ_LPT2,
  IRQ_FLOPPY_DISK,
  IRQ_LPT1,
  IRQ_CMOS_RTC,
  IRQ_FREE1,
  IRQ_FREE2,
  IRQ_FREE3,
  IRQ_PS2_MOUSE,
  IRQ_FPU = 13,
  IRQ_COPROCESSOR = 13,
  IRQ_INTER_PROCESSOR = 13,
  IRQ_PRIMARY_ATA,
  IRQ_SECONDARY_ATA,
} irq_hw_t;

// API

/// @brief interrupt parameter frame, use as argument in ISRs
typedef struct __attribute__((packed)) int_frame {
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
  uint32_t sp;
  uint32_t ss;
} int_frame;

/// Set attributes needed to iret and save the stack
#define INTERRUPT                                                              \
  __attribute__((target("general-regs-only"))) __attribute__((interrupt))

/// @brief initialize the interrupt driver
void int_init();

// HW //

/// @brief Signal the end of an hardware interrupt
void int_irq_end(irq_hw_t irq);

/// @brief Remove hardware interrupt at specified IRQ
/// @param irq IRQ to remove the interrupt at
void int_irq_del(irq_hw_t irq);

/// @brief Setup hardware interrupt
/// @param irq IRQ to setup the interrupt at
/// @param interrupt_handler interrupt handler, call `int_irq_end` when finished
void int_irq_add(irq_hw_t irq, void (*interrupt_handler)());

// SW //

/// @brief add an arbritary interrupt, (set in the IDT after SYSCALL (128))
/// @param nbr number of our interrupt (up to 127)(remaped in the IDT)
/// @param int_handler
void int_add(char nbr, INTERRUPT void (*int_handler)(int_frame *frame));

/// @brief delete an arbitrary interrupt, (set in the IDT after SYSCALL (128))
/// @param nbr number of our interrupt (up to 127)(remaped in the IDT)
void int_del(char nbr);

#endif
