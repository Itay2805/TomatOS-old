#include "syscalls.h"

#include <syscall.h>

#include "interrupt.h"

static void syscall_handle(registers_t regs) {

}

void initialize_syscalls(void) {
	register_interrupt_handler(0x80, &syscall_handle);
}
