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

/// @brief init the idt
void init_interrupts();

/// Set attributes needed to iret and save the stack
#define INTERRUPT                                                              \
  __attribute__((target("general-regs-only"))) __attribute__((interrupt))

// Hardware interrupts
// int_add_hw(enum irq_hw);

// Software interrupts
// init_add_hw();

#endif
