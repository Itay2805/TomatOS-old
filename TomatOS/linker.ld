ENTRY(bootloader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
	. = 0x0100000;
	
	.text :
	{
		*(.multiboot)
		*(.text)
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
}