#include "interrupts.h"
#include "idt.h"
#include "pic.h"

void init_interrupts() {
  init_pic(IDT_PIC_OFFSET, IDT_PIC_OFFSET + 7);
  init_idt();

  __asm__("sti"); // S(e)T I(nterrupts)
}

void int_add_irq(enum irq_hw, void (*interrupt_handler)()) {
  // TODO:
  // - unmask IRQ in PIC
  // - add IRQ handler to IDT
}

void int_del_irq(enum irq_hw) {
  // TODO:
  // - mask IRQ in PIC
  // - remove IRQ from IDT
}
