#include <stdint.h>

#include "gdt/gdt.h"
#include "interrupts/idt.h"

#include <string.h>

#include <core/graphics/term.h>
#include <core/memory/paging.h>
#include <core/memory/heap.h>

#include <core/process/syscall.h>
#include <core/process/process.h>
#include <core/process/perm.h>

extern void kmain();

void kernel_boot(const void* multiboot_structure, uint32_t multiboot_magic) {
	UNUSED(multiboot_structure);
	UNUSED(multiboot_magic);
	
	// the kernel boot will basically load the basics needed for the kernel
	// to run like segments all the way to registering syscalls

	term_init();
	term_clear();
	term_write("Booting into TomatKernel...\n\n");

	term_write("[TomatoBoot] initializing paging\n");
	paging_init();

	term_write("[TomatoBoot] initializing gdt\n");
	gdt_init();
	term_write("[TomatoBoot] initializing idt\n");
	idt_init();
	term_write("[TomatoBoot] initializing syscalls\n");
	syscall_init();

	term_write("[TomatoBoot] registering terminal and window syscalls\n");
	term_register_syscalls();
	window_register_syscalls();

	term_write("[TomatoBoot] registering heap sysalls\n");
	heap_register_syscalls();

	term_write("[TomatoBoot] registering process syscalls and initializing alive\n");
	process_init();

	// from here we are technically done with boot and should
	// only focus on loading libraries and such
	term_clear();
	term_set_cursor_pos(0, 0);
	term_write("Welcome to TomatKernel!\n\n");
	
	term_write("[TomatoKernel] Loading core service\n");
	process_t foreground;
	process_create(&foreground, kmain, USER_ALIVE, true); // this will run as alive just because it must have the kernel segments
	process_start(&foreground);

	term_write("[TomatoKernel] Starting alive\n");
	asm volatile
		("int $0x80"
			:
			: "a"(SYSCALL_START_ALIVE)
			);

	kpanic("SHOULD NOT HAVE REACHED HERE!!!");
}