#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/// @brief Standard ISA IRQs
enum irq_hw {
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
};

// API

/// Set attributes needed to iret and save the stack
#define INTERRUPT                                                              \
  __attribute__((target("general-regs-only"))) __attribute__((interrupt))

/// @brief initialize the interrupt driver
void init_interrupts();

// Hardware interrupts

/// @brief Setup hardware interrupt
/// @param irq_hw IRQ to setup the interrupt at
/// @param interrupt_handler function to call when the interrupt is raised
void int_add_irq(enum irq_hw, void (*interrupt_handler)());

/// @brief Remove hardware interrupt at specified IRQ
/// @param irq_hw IRQ to remove the interrupt at
void int_del_irq(enum irq_hw);

// TODO:
// - mechanism to validate IRQ by checking IRR (to catch spurious IRQs)

// Software interrupts
// init_add_sw();

#endif
