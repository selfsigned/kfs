#include "idt.h"

struct interrupt_desc idt[IDT_NB_ENTRIES];
struct idt_ptr idt_ptr;

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

void fill_idt_entries(){
  memset(&idt, 0, sizeof(struct interrupt_desc) * 256);

}

void init_idt() {
  idt_ptr.limit = (sizeof(struct interrupt_desc) * IDT_NB_ENTRIES) - 1;
  idt_ptr.base = (uint32_t)&idt;
  
  fill_idt_entries();
}
