#ifndef TOMATO_TERM_H
#define TOMATO_TERM_H

#include <stdint.h>

#include <tomato/syscalls.h>
#include <tomato/windows.h>

#if __cplusplus
extern "C" {
#endif
	
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

	static inline void tomato_term_set_cursor_pos(uint16_t x, uint16_t y) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_SET_CURSOR_POS), "b"(x), "c"(y));
	}

	static inline uint16_t tomato_term_get_cursor_x() {
		uint16_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_GET_CURSOR_X));
		return ret;
	}

	static inline uint16_t tomato_term_get_cursor_y() {
		uint16_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_GET_CURSOR_Y));
		return ret;
	}

	static inline void tomato_term_scroll(uint16_t n) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_SCROLL), "b"(n));
	}

	static inline uint16_t tomato_term_width() {
		uint16_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_WIDTH));
		return ret;
	}

	static inline uint16_t tomato_term_height() {
		uint16_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_HEIGHT));
		return ret;
	}

	static inline void tomato_term_clear_line(uint16_t n) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_CLEAR_LINE), "b"(n));
	}

	static inline tomato_window_t tomato_term_native() {
		tomato_window_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_NATIVE));
		return ret;
	}

	static inline tomato_window_t tomato_term_current() {
		tomato_window_t ret = 0;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_TERM_CURRENT));
		return ret;
	}

	static inline void tomato_term_redirect(tomato_window_t window) {
		asm("int $0x80" : : "a"(SYSCALL_TERM_REDIRECT), "b"(window));
	}

#if __cplusplus
}
#endif

#endif