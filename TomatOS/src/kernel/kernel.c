
#include <stdint.h>

#include <system.h>

#include "gdt.h"
#include "interrupt.h"
#include "syscalls.h"

typedef void(*constructor)();
constructor start_ctors;
constructor end_ctors;
void callConstructors() {
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

void kmain(const void* multiboot_structure, uint32_t multiboot_magic) {
	initialize_gdt();
	initialize_syscalls();
	initialize_interrupts();

	asm("int $3");
	//tomato_term_write("test");
}
