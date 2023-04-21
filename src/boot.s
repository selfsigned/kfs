/* Declare constants for the multiboot header. 
- Setting flags option ALIGN and MEMINFO to true (precise that all modules should be align on page boundaries 
and a description of the memory should beprovided to the kernel) 
- Set the magic which is a special number indicating it is a multiboot header otherwise wont be recognized
- Cheksum to verify header integrity
*/

.set ALIGN,    1<<0             
.set MEMINFO,  1<<1             
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 


/* Store variables of multiboot header in a multiboot section */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/* Initialize space for a stack of 32KiB, for the kernel */

.section .bss
.align 16
stack_bottom:
.skip 32768 # 32 KiB
stack_top:


/* Executable code of the kernel will be put in this section, as start is define as entry by linker it
will execute this code when the kernel starts */

.section .text
.global _start
.type _start, @function
_start:


/* Initialize stack pointers to the beginning of stack memory */

	mov $stack_top, %esp


/* Calls the function of the kernel called kernel_main (in kernel.c) */

	call kernel_main


	/* Create an infinite loop (disable iterrupt and wait for interrupt) */

	cli
1:	hlt
	jmp 1b
