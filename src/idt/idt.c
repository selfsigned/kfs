#include "idt.h"

struct interrupt_desc idt[IDT_NB_ENTRIES] = {};
struct idt_ptr idt_ptr;

/// @brief fill the interrupt descriptor passed as 1st parameter
/// @param interrupt_descriptor interrupt descriptor to set
/// @param base address of the ISR to call to handle this interrupt
/// @param segment_selector valid segment selector (ex code segm selector)
/// @param access interrupt type and permissions
void create_interrupt_desc(struct interrupt_desc *interrupt_desc,
                                uint32_t base, uint16_t segment_selector, uint8_t access) 
{
	interrupt_desc->base_low = (base & 0xFFFF);
	interrupt_desc->base_high = (base >> 16) & 0xFFFF;

	interrupt_desc->segment_selector = segment_selector;
	interrupt_desc->reserved = 0;
  interrupt_desc->access = access;
}
/* -- IDT GATES  
0 - 31 : system traps and exceptions
32 - 127  : device interrupts (IRQs?)
128 : int80 syscall interface
 129 - 255 : other interrupts
*/

/* -- EXCEPTIONS
0 - Division error 
1 - Debug 	
2 - Non-maskable Interrupt
3 - Breakpoint 
4 - Overflow 
5 - Bound Range Exceeded 
6 - Invalid Opcode 
7 - Device Not Available 
8 - Double Fault 
9 - Coprocessor Segment Overrun 
10 - Invalid TSS 
11 - Segment Not Present 	
12 - Stack-Segment Fault
13 - General Protection Fault 
14 - Page Fault 
15 - Reserved 
16 - x87 Floating-Point Exception 
17 - Alignment Check 	
18 - Machine Check 
19 - SIMD Floating-Point Exception 
20 - Virtualization Exception 	
21 - Control Protection Exception 
22 27 - Reserved 	22-27 
28 - Hypervisor Injection Exception 
29 - VMM Communication Exception 
30 - Security Exception
31 - Reserved
Triple Fault 	
FPU Error Interrupt 	IRQ 13 */

/* -- IRQ HARDWARE
   0 – system timer 
   1 –  PS2 keyboard 
   2 – cascaded 
   3 – COM2 
   4 – COM1
   5 – LPT2 
   6 – floppy disk 
   7 – LPT1 
   8 – real-time clock
   9 – Free for peripherals SCSI/NIC 
   10 – Free for peripherals SCSI/NIC
   11 – Free for peripherals SCSI/NIC
   12 – PS2 mouse
   13 – FPU/Coprocessor, Inter-processor 
   14 – primary ATA Hard I 
   15 – secondary ATA Ha Disk 
*/

/* synchronous(exception) / asynchronous
maskable / non maskable
 */
/* Exceptions : faults, trap, aborts, int n */


/// Segment selector 0x08, code segment (where interrupts handler reside)
/// address: adress of the handler fonction to use
/// Access : 0x8E for kernel access (ring 0) 0x60 for user access (ring 3)
void fill_idt_entries(){
//  memset(&idt, 0, sizeof(struct interrupt_desc) * 256);
  create_interrupt_desc(&idt[0], 0, 0x08, 0x8E);
  //create_interrupt_desc(&idt[0], division_error, 0x08, 0x8E);
  //create_interrupt_desc(&idt[1], debug, 0x08, 0x8E); 
  //create_interrupt_desc(&idt[2], non_masquable_interrupt, 0x08, 0x8E); 
  //create_interrupt_desc(&idt[3], breakpoint, 0x08, 0x8E); 
  //create_interrupt_desc(&idt[4], overflow, 0x08, 0x8E);
}

void init_idt() {
  idt_ptr.limit = (sizeof(struct interrupt_desc) * IDT_NB_ENTRIES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  
  fill_idt_entries();
}
