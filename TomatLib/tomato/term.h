#ifndef TOMATO_TERM_H
#define TOMATO_TERM_H

#include <stdint.h>

#include <tomato/syscalls.h>

#if __cplusplus
extern "C" {
#endif

	// TODO: How to implement terminal override accross processes

	static inline void tomato_term_write(const char* data) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_WRITE), "b"(data));
	}
	
	static inline void tomato_term_clear() {
		asm("int $0x80" : : "a"(SYSCALL_TERM_CLEAR));
	}
	
	static inline void tomato_term_set_text_color(uint8_t color) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_SET_TEXT_COLOR), "b"(color));
	}
	
	static inline void tomato_term_set_background_color(uint8_t color) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_SET_BACKGROUND_COLOR), "b"(color));
	}
	
	static inline uint8_t tomato_term_get_text_color() {
		uint8_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_GET_TEXT_COLOR));
		return ret;
	}

	static inline uint8_t tomato_term_get_background_color() {
		uint8_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_GET_BACKGROUND_COLOR));
		return ret;
	}

	static inline void tomato_term_set_cursor_pos(uint8_t x, uint8_t y) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_SET_CURSOR_POS), "b"(x), "c"(y));
	}

	static inline uint8_t tomato_term_get_cursor_x() {
		uint8_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_GET_CURSOR_X));
		return ret;
	}

	static inline uint8_t tomato_term_get_cursor_y() {
		uint8_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_GET_CURSOR_Y));
		return ret;
	}

	static inline void tomato_term_scroll(uint8_t n) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_SCROLL), "b"(n));
	}

	static inline uint8_t tomato_term_width() {
		uint8_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_WIDTH));
		return ret;
	}

	static inline uint8_t tomato_term_height() {
		uint8_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_HEIGHT));
		return ret;
	}

	static inline void tomato_term_clear_line(uint8_t n) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_CLEAR_LINE), "b"(n));
	}

#if __cplusplus
}
#endif

#endif