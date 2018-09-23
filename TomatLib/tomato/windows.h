#ifndef TOMATO_WINDOW_H
#define TOMATO_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

typedef uintptr_t tomato_window_t;

#ifdef __cplusplus
extern "C" {
#endif
	
	inline static tomato_window_t tomato_window_create(tomato_window_t parent, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool vis) {
		tomato_window_t window;
		asm("int $0x80" : "=a"(window) : "a"(SYSCALL_WINDOW_CREATE), "b"(parent), "c"(x), "d"(y), "S"(width), "D"(height));
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_SET_VISIBLE), "b"(vis));
		return window;
	}
	
	inline static void tomato_window_delete(tomato_window_t window) {
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_DELETE), "b"(window));
	}

	inline static void tomato_window_write(tomato_window_t window, const char* data) {
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_DELETE), "b"(window), "c"(data));
	}

	inline static void tomato_window_clear(tomato_window_t window) {
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_DELETE), "b"(window), "c"(data));
	}

	inline static void tomato_window_set_text_color(tomato_window_t window, uint8_t color) {
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_SET_TEXT_COLOR), "b"(window), "c"(color));
	}

	inline static void tomato_window_set_background_color(tomato_window_t window, uint8_t color) {
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_SET_BACKGROUND_COLOR), "b"(window), "c"(color));
	}

	inline static uint8_t tomato_window_get_text_color(tomato_window_t window) {
		uint8_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_TEXT_COLOR), "b"(window));
		return ret;
	}

	inline static uint8_t tomato_window_get_background_color(tomato_window_t window) {
		uint8_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_BACKGROUND_COLOR), "b"(window));
		return ret;
	}

	inline static void tomato_window_set_cursor_pos(tomato_window_t window, uint16_t x, uint16_t y) {
		asm("int $0x80" : : "a"(SYSCALL_WINDOW_SET_CURSOR_POS), "b"(window), "c"(x), "d"(y));
	}

	inline static uint16_t tomato_window_get_cursor_x(tomato_window_t window) {
		uint16_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_CURSOR_X), "b"(window));
		return ret;
	}

	inline static uint16_t tomato_window_get_cursor_y(tomato_window_t window) {
		uint16_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_CURSOR_Y), "b"(window));
		return ret;
	}

	inline static void tomato_window_scroll(tomato_window_t window, uint16_t n) {
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_SCROLL), "b"(window), "c"(n));
	}

	inline static uint16_t tomato_window_width(tomato_window_t window) {
		uint16_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_CURSOR_Y), "b"(window));
		return ret;
	}

	inline static uint16_t tomato_window_height(tomato_window_t window) {
		uint16_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_CURSOR_Y), "b"(window));
		return ret;
	}

	inline static void tomato_window_clear_line(tomato_window_t window, uint16_t n) {
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_CLEAR_LINE), "b"(window), "c"(n));
	}
	
	inline static void tomato_window_set_visible(tomato_window_t window, bool vis) {
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_SET_VISIBLE), "b"(window), "c"(vis));
	}

	inline static void tomato_window_redraw(tomato_window_t window) {
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_REDRAW), "b"(window));
	}

	inline static uint16_t tomato_window_get_x(tomato_window_t window) {
		uint16_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_Y), "b"(window));
		return ret;
	}

	inline static uint16_t tomato_window_get_y(tomato_window_t window) {
		uint16_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_GET_X), "b"(window));
		return ret;
	}

	inline static void tomato_window_restore_cursor(tomato_window_t window) {
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_RESTORE_CURSOR), "b"(window));
	}

	inline static void tomato_window_reposition(tomato_window_t window, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_WINDOW_REPOSITION), "b"(window), "c"(x), "d"(y), "S"(width), "D"(height));
	}
	
#ifdef __cplusplus
}
#endif

#endif