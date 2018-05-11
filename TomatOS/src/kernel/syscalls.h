#ifndef KERNEL_SYSCALLS_H
#define KERNEL_SYSCALLS_H

#include "interrupt.h"

typedef uint32_t(*syscall_handle)(registers_t regs);

void initialize_syscalls(void);
void register_syscall(int syscall, syscall_handle handle);

#endif

