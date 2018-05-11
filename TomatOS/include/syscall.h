#ifndef TOMATO_SYSCALL_H
#define TOMATO_SYSCALL_H

#include <stdint.h>
#include <stdbool.h>

#define TOMATO_SYSCALL_TERM_WRITE 1
#define TOMATO_SYSCALL_TERM_WRITE 2
#define TOMATO_SYSCALL_TERM_WRITE 3
#define TOMATO_SYSCALL_TERM_WRITE 4
#define TOMATO_SYSCALL_TERM_WRITE 5
#define TOMATO_SYSCALL_TERM_WRITE 6
#define TOMATO_SYSCALL_TERM_WRITE 7
#define TOMATO_SYSCALL_TERM_WRITE 8
#define TOMATO_SYSCALL_TERM_WRITE 9
#define TOMATO_SYSCALL_TERM_WRITE 10
#define TOMATO_SYSCALL_TERM_WRITE 11
#define TOMATO_SYSCALL_TERM_WRITE 12
#define TOMATO_SYSCALL_TERM_WRITE 13

inline static uint32_t tomato_syscall(uint32_t id) {
	uint32_t ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(id));
	return ret;
}

inline static uint32_t tomato_syscall_1p(uint32_t id, uint32_t p) {
	uint32_t ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(id), "b"(p));
	return ret;
}

#endif