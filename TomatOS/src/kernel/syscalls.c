#include "syscalls.h"

#include <stddef.h>
#include <syscall.h>

#include "interrupt.h"
#include "port.h"

/////////////////////////////////////////////////////
//// syscall handle
/////////////////////////////////////////////////////

static syscall_handle handlers[256];

static void syscall_handler(registers_t regs) {
	if (handlers[regs.eax] != NULL) {
		uint32_t eax = handlers[regs.eax](regs);
		ASM(asm volatile("mov %0, %%eax" : : "r"(eax)));
	}
}

void register_syscall(int syscall, syscall_handle handle) {
	handlers[syscall] = handle;
}

void initialize_syscalls(void) {
	register_interrupt_handler(0x80, &syscall_handler);


}
