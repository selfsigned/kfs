.global _gdt_flush /* Export the symbol gdt_flush mean can be use elsewhere in c for example*/
.extern gdt_ptr /* Means gdt_ptr symbol is defined elsewhere */

_gdt_flush:
    lgdt (gdt_ptr) /* Load gdt_ptr in the gdt register */

    mov $0x10, %ax /* 0x10 is segment selector for data segment */ 
    mov %ax, %ds /* Place data segment selector in apporpriate segment registers ( ds : data segment, ss : stack segment ... ) */
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $flush2 /* Jump to 0x08 which is the code segment, in flush2 */
flush2:
    ret /* Then return in c code of our kernel */
