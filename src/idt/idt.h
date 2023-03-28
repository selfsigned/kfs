#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_interrupt_desc {
   uint16_t base_low; /// address of the ISR (interrupt service routine) to jump to when this interrupt is called
   uint16_t segment_selector;  ///valid gdt segment               
   uint8_t  reserved; /// set to 0 
   uint8_t  access;               
   uint16_t base_high; /// address is divided in two parts
} __attribute__((packed));

struct idt_ptr {
   uint16_t limit; /// size of idt
   uint32_t base; /// address
} __attribute__((packed));

void init_idt();

#endif
