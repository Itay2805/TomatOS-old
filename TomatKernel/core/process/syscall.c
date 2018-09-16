#include "syscall.h"

#include <boot/interrupts/isr.h>

#include <string.h>

static syscall_handler_t handlers[SYSCALL_END];

static void syscall_handler(registers_t* regs) {
	if (regs->eax < SYSCALL_END && handlers[regs->eax] != NULL) {
		handlers[regs->eax](regs);
	}
}

void syscall_init(void) {
	register_irq_handler(0x80, syscall_handler);

	memset(handlers, 0, sizeof(handlers));
}

void syscall_register(syscall_kind_t kind, syscall_handler_t handler) {
	handlers[kind] = handler;
}
