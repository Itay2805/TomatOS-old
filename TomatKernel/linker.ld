ENTRY(bootloader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
	. = 0x0100000;

	tomatkernel_start = .;

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
	
	tomatkernel_end = .;
	tomatkernel_size = tomatkernel_end - tomatkernel_start;
}