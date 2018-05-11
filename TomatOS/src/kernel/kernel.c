
#include <stdint.h>

#include "gdt.h"
#include "interrupt.h"
#include "syscalls.h"

#include "syscalls/term.h"

typedef void(*constructor)();
constructor start_ctors;
constructor end_ctors;
void callConstructors() {
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

// os entry point
extern void startup();

void kmain(const void* multiboot_structure, uint32_t multiboot_magic) {
	// initialize the kernel
	initialize_gdt();
	initialize_interrupts();
	
	// initialize syscalls
	initialize_syscalls();
	syscall_term_init();

	// call the os startup
	startup();
}
