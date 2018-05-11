
#include <stdint.h>

#include <system.h>

#include "gdt.h"
#include "interrupt.h"
#include "syscalls.h"

void kmain(const void* multiboot_structure, uint32_t multiboot_magic) {
	initialize_gdt();
	initialize_syscalls();
	initialize_interrupts();

	int val = 4;
	char* text = "test";
	asm("int $3");
	//tomato_term_write("test");
}
