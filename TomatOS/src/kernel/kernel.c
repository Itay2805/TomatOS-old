
#include <stdint.h>

#include "gdt.h"
#include "interrupt.h"
#include "heap.h"
#include "syscalls.h"

#include "syscalls/term.h"
#include "syscalls/os.h"
#include "syscalls/fs.h"

#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "drivers/disk.h"
#include "drivers/mouse.h"

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

	// initialize heap
	uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
	uint32_t heap_start = 10 * 1024 * 1024;
	initialize_heap(heap_start, (*memupper) * 1024 - heap_start - 10 * 1024);
	
	// initialize syscalls
	initialize_syscalls();
	syscall_term_init();
	syscall_os_init();

	// initialize driver
	driver_timer_init();
	driver_keyboard_init();
	driver_mouse_init();
	driver_disk_init();

	// initialize syscalls that depend on drivers
	syscall_fs_init();

	// reset terminal
	term_kreset();

	driver_mouse_draw();

	// call the os startup
	startup();
}
