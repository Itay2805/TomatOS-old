ENTRY(kmain)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
	. = 0x0100000;

	kernel_start = .;

	.text :
	{
		*(.multiboot)
		*(.text*)
		*(.rodata)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}
	
	kernel_end = .;
	kernel_size = kernel_end - kernel_start;
}