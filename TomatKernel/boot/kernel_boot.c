#include <stdint.h>

#include "gdt/gdt.h"
#include "interrupts/idt.h"

#include <string.h>

#include <core/term.h>
#include <core/memory/paging.h>
#include <core/memory/heap.h>
#include <core/process/syscall.h>
#include <core/process/process.h>

extern void kmain();

// the kernel boot is for initializing core drivers
void kernel_boot(const void* multiboot_structure, uint32_t multiboot_magic) {
	UNUSED(multiboot_structure);
	UNUSED(multiboot_magic);

	// initialize terminal
	term_init();

	term_clear();
	term_write("Booting into TomatKernel\n\n");

	// init the paging
	paging_init();

	// initialize gdt and idt
    gdt_init();
    idt_init();
	syscall_init();

	// initialize syscalls for term
	term_init_syscalls();

	// initialize kernel heap
	heap_init();
	process_init();

	term_clear();

	term_write("Welcome to TomatKernel!");
	
	// initialize a foreground process
	process_t foreground;
	process_create(&foreground, kmain, true);
	process_start(&foreground);

	asm volatile
		("int $0x80"
			:
			: "a"(SYSCALL_START_ALIVE)
			);

	kpanic("SHOULD NOT HAVE REACHED HERE!!!");
}