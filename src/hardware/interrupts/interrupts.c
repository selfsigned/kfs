#include "interrupts.h"
#include "idt.h"
#include "pic.h"

/// @brief
uint32_t int_irq_spuriousnbr = 0;

//  _      _ _
// (_)_ _ (_) |_
// | | ' \| |  _|
// |_|_||_|_|\__|

void int_init() {
  pic_init(IDT_PIC_OFFSET, IDT_PIC_OFFSET + 8);
  idt_init();

  __asm__("sti"); // S(e)T I(nterrupts)
  INFO_MSG("IDT loaded, IF set");
}

//  _                _
// | |_  __ _ _ _ __| |_ __ ____ _ _ _ ___
// | ' \/ _` | '_/ _` \ V  V / _` | '_/ -_)
// |_||_\__,_|_| \__,_|\_/\_/\__,_|_| \___|

irq_hw_t int_irq_get_irr() { return pic_get_irq_reg(OCW3_READ_IRR); }
irq_hw_t int_irq_get_isr() { return pic_get_irq_reg(OCW3_READ_ISR); }

void int_irq_end(irq_hw_t irq) { pic_send_eoi(irq); }

void int_irq_add(irq_hw_t irq, void (*int_handler)(int_frame *frame)) {
  idt_add_gate(IDT_PIC_OFFSET + irq, int_handler, INT_GATE_FLAGS);
  pic_unmask(irq);
}

void int_irq_del(irq_hw_t irq) {
  pic_mask(irq);
  idt_del_gate(IDT_PIC_OFFSET + irq);
}

//           __ _
//  ___ ___ / _| |___ __ ____ _ _ _ ___
// (_-</ _ \  _|  _\ V  V / _` | '_/ -_)
// /__/\___/_|  \__|\_/\_/\__,_|_| \___|

void int_add(char nbr, void (*int_handler)(int_frame *frame)) {
  idt_add_gate(IDT_SW_OFFSET + nbr, int_handler, INT_GATE_FLAGS);
}

void int_del(char nbr) { idt_del_gate(IDT_SW_OFFSET + nbr); }
