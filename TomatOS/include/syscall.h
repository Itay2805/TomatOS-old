#ifndef TOMATO_SYSCALL_H
#define TOMATO_SYSCALL_H

#include <stdint.h>
#include <stdbool.h>

//// 0x01 - 0x0F - terminal
#define TOMATO_SYSCALL_TERM_WRITE					0x01
#define TOMATO_SYSCALL_TERM_CLEAR					0x02
#define TOMATO_SYSCALL_TERM_SET_TEXT_COLOR			0x03
#define TOMATO_SYSCALL_TERM_SET_BACKGROUND_COLOR	0x04
#define TOMATO_SYSCALL_TERM_GET_TEXT_COLOR			0x05
#define TOMATO_SYSCALL_TERM_GET_BACKGROUND_COLOR	0x06
#define TOMATO_SYSCALL_TERM_SET_CURSOR_POS			0x07
#define TOMATO_SYSCALL_TERM_GET_CURSOR_X			0x08
#define TOMATO_SYSCALL_TERM_GET_CUSROR_Y			0x09
#define TOMATO_SYSCALL_TERM_GET_WIDTH				0x0A
#define TOMATO_SYSCALL_TERM_GET_HEIGHT				0x0B
#define TOMATO_SYSCALL_TERM_SCROLL					0x0C
#define TOMATO_SYSCALL_TERM_CLEAR_LINE				0x0D
#define TOMATO_SYSCALL_TERM_CLEAR_BLIT				0x0E

//// 0x10 - 0x14 - heap
#define TOMATO_SYSCALL_HEAP_ALLOCATE				0x10
#define TOMATO_SYSCALL_HEAP_REALLOCATE				0x11
#define TOMATO_SYSCALL_HEAP_FREE					0x12

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

inline static uint32_t tomato_syscall_2p(uint32_t id, uint32_t p1, uint32_t p2) {
	uint32_t ret;
	asm volatile("int $0x80" : "=a"(ret) : "a"(id), "b"(p1), "c"(p2));
	return ret;
}

#endif