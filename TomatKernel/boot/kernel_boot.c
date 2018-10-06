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

#include "multiboot.h"

#include <core/drivers/ps2_keyboard.h>

#include <core/timer.h>

extern void kmain();

void kernel_boot(const multiboot_t* multiboot_structure, uint32_t multiboot_magic) {
	// the kernel boot will basically load the basics needed for the kernel
	// to run like segments all the way to registering syscalls

	term_init();
	term_clear();

	// make sure this is a proper bootloader
	if(multiboot_magic != 0x2BADB002) {
		kpanic("Not loaded by a proper Bootloader\n");
	}

	multiboot_mem_t* mem = multiboot_get_mem(multiboot_structure);
	multiboot_framebuffer_t* framebuffer = multiboot_get_framebuffer(multiboot_structure);
	// if the framebuffer is not available
	// we will need to use the VGA drivers to enable graphcis mode...

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

	term_write("[TomatoBoot] registering and initializing timer\n");
	timer_init();

	term_write("[TomatoBoot] registering and initializing PS2 keyboard driver\n");
	ps2_keyboard_init();

	// from here we are technically done with boot and should
	// only focus on loading libraries and such
	term_clear();
	term_set_cursor_pos(0, 0);
	term_write("Welcome to TomatKernel!\n\n");
	
	term_write("[TomatoKernel] Loading core program\n");
	// TODO: This should look at configuration to load the assembly of the core program
	
	process_t foreground;
	process_create(&foreground, kmain, USER_ALIVE, true); // this will run as alive just because it must have the kernel segments
	foreground.core = true;
	process_start(&foreground);

	term_write("[TomatoKernel] Starting alive\n");
	__asm("int $0x80" : : "a"(SYSCALL_START));
	
	// Once alive has started it means that the control is given to the core program
	// or to the alive program. It should never return to here (Maybe make so it will 
	// only return if we want to close the program?)
	kpanic("SHOULD NOT HAVE REACHED HERE!!!");
}