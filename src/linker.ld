/* Entry point */
ENTRY(_start)
 
/* Places the differents sections */
SECTIONS
{
	. = 1M;
 
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot) /* Multiboot header */
		*(.text) /* Code section */
	}
 
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)	/* Read-only data */
	}
 
	.data BLOCK(4K) : ALIGN(4K) 
	{
		*(.data) /* Read-write initialized data */
	}
 
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON) /* Read-write uninitialized data */
		*(.bss) /* Stack of the kernel */
	}
}