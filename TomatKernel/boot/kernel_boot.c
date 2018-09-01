#include <stdint.h>

#include "gdt/gdt.h"
#include "interrupts/idt.h"

#include <string.h>

#include <core/term.h>
#include <core/memory/paging.h>

extern void kmain();

// the kernel boot is for initializing core drivers
void kernel_boot(const void* multiboot_structure, uint32_t multiboot_magic) {
	// initialize terminal
	term_init();

	term_clear();
	term_write("Booting into TomatKernel\n\n");

	// init the paging
	paging_init();

	// initialize gdt and idt
    gdt_init();
    idt_init();

	// initialize kernel heap
	term_write("[kheap] Initializing\n");
	uintptr_t kernelHeapStart = (((size_t)&tomatkernel_end + 1024 * 1024) >> 12) << 12;
	heap_create(kheap(), kernelHeapStart);
	
	term_clear();

	term_write("Welcome to TomatKernel!");

	bool running = true;
	while (running) {
		
	}
}