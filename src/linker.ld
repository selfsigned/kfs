/* Entry point */
ENTRY(_start)
 
/* Places the differents sections */
SECTIONS
{
	/* Putting our kernel in the extended memory area */
	/* https://wiki.osdev.org/Memory_Map_(x86)#Extended_Memory_.28.3E_1_MiB.29 */
	. = 1M;
 
	/* putting our multiboot header at the start to make sure grub picks it up */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot) /* Multiboot header */
		*(.text) /* Code section */
	}
 
	/* gcc will take care of populating the other sections */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON) /* Read-write uninitialized data */
		*(.bss) /* Stack of the kernel */
	}
}