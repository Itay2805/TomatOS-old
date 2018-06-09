
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

	// initialize driver
	driver_timer_init();
	driver_keyboard_init();
	driver_disk_init();
	
	/*char buf[] = "Another Test";
	term_kwrite("before write/read test: ");
	term_kwrite(buf);
	term_kwrite("\n");
	driver_disk_write(512, buf, sizeof(buf));
	driver_disk_read(512, buf, sizeof(buf));
	term_kwrite("after write/read test: ");
	term_kwrite(buf);
	term_kwrite("\n");*/

	syscall_fs_init();

	// reset terminal
	term_kreset();
	
	// call the os startup
	// startup();
}
