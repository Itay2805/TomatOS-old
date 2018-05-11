#ifndef TOMATO_SYSCALL_H
#define TOMATO_SYSCALL_H

#include <stdint.h>
#include <stdbool.h>

#define TOMATO_SYSCALL_TERM_WRITE 4

inline static void tomato_syscall(uint32_t id) {
	asm("mov %0, %%eax" : : "r"(id));
	asm("int $128");
}

inline static void tomato_syscall_1p(uint32_t id, uint32_t p) {
	asm("mov %0, %%eax" : : "r"(id));
	asm("mov %0, %%ebx" : : "r"(p));
	asm("int $128");
}

#endif