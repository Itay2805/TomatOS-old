#include "os.h"

#include <stddef.h>

#include <tomato.h>

#include "../interrupt.h"
#include "../syscalls.h"
#include "../port.h"

static const char* version_str = "TomatOS - 2.0";

#include "term.h"

static void syscall_version(registers_t* regs) {
	regs->eax = (uint32_t)version_str;
}

void syscall_os_init(void) {
	term_kwrite("syscall init: registering OS syscalls\n");
	register_syscall(TOMATO_SYSCALL_OS_VERSION, syscall_version);
}
