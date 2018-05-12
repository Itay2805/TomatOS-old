#ifndef TOMATO_SYSCALL_H
#define TOMATO_SYSCALL_H

#include <stdint.h>
#include <stdbool.h>

//// 0x01 - 0x0F - os
#define TOMATO_SYSCALL_OS_VERSION					0x01
#define TOMATO_SYSCALL_OS_PULL_EVENT_BLOCKING		0x02
#define TOMATO_SYSCALL_OS_QUEUE_EVENT				0x03
#define TOMATO_SYSCALL_OS_START_TIMER				0x04
#define TOMATO_SYSCALL_OS_CANCEL_TIMER				0x05
#define TOMATO_SYSCALL_OS_SHUTDOWN					0x06
#define TOMATO_SYSCALL_OS_REBOOT					0x07

//// 0x10 - 0x1F - terminal
#define TOMATO_SYSCALL_TERM_WRITE					0x11
#define TOMATO_SYSCALL_TERM_CLEAR					0x12
#define TOMATO_SYSCALL_TERM_SET_TEXT_COLOR			0x13
#define TOMATO_SYSCALL_TERM_SET_BACKGROUND_COLOR	0x14
#define TOMATO_SYSCALL_TERM_GET_TEXT_COLOR			0x15
#define TOMATO_SYSCALL_TERM_GET_BACKGROUND_COLOR	0x16
#define TOMATO_SYSCALL_TERM_SET_CURSOR_POS			0x17
#define TOMATO_SYSCALL_TERM_GET_CURSOR_X			0x18
#define TOMATO_SYSCALL_TERM_GET_CUSROR_Y			0x19
#define TOMATO_SYSCALL_TERM_GET_WIDTH				0x1A
#define TOMATO_SYSCALL_TERM_GET_HEIGHT				0x1B
#define TOMATO_SYSCALL_TERM_SCROLL					0x1C
#define TOMATO_SYSCALL_TERM_CLEAR_LINE				0x1D
#define TOMATO_SYSCALL_TERM_CLEAR_BLIT				0x1E

//// 0x20 - 0x24 - heap
#define TOMATO_SYSCALL_HEAP_ALLOCATE				0x20
#define TOMATO_SYSCALL_HEAP_REALLOCATE				0x21
#define TOMATO_SYSCALL_HEAP_FREE					0x22

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