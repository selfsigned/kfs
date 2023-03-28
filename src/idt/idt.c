#include "idt.h"

struct idt_interrupt_desc idt[IDT_NB_ENTRIES];
struct idt_ptr idt_ptr;

void fill_idt_entries(){
  
}

void init_idt() {
  idt_ptr.limit = (sizeof(struct idt_interrupt_desc) * IDT_NB_ENTRIES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  fill_idt_entries();
}
