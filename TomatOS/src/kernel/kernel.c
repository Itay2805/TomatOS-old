
#include <stdint.h>

#include "gdt.h"
#include "interrupt.h"
#include "heap.h"
#include "syscalls.h"

#include "syscalls/term.h"
#include "syscalls/os.h"

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
	UNUSED(multiboot_magic);

	// initialize the kernel
	initialize_gdt();
	initialize_interrupts();
	
	// initialize syscalls
	initialize_syscalls();
	syscall_term_init();
	syscall_os_init();

	// initialize heap
	uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
	uint32_t heap_start = 10 * 1024 * 1024;
	initialize_heap(heap_start, (*memupper)*1024 - heap_start - 10 * 1024);

	// reset terminal
	term_kreset();
	
	// call the os startup
	startup();
}
