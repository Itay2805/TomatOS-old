#ifndef TOMATO_H
#define TOMATO_H

// this will contain all the system calls
// for C++ headers look at the apis include dir

#include <syscall.h>

/////////////////////////////////////////////////////
//// term API
/////////////////////////////////////////////////////

inline void tomato_term_write(const char* text) { 
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_WRITE, (uint32_t)text);
}

inline void tomato_term_clear(void) {
	tomato_syscall(TOMATO_SYSCALL_TERM_CLEAR);
}

inline void tomato_term_set_text_color(uint8_t color) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_SET_TEXT_COLOR, (uint32_t)color);
}

inline void tomato_term_set_background_color(uint8_t color) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_SET_BACKGROUND_COLOR, (uint32_t)color);
}

inline uint8_t tomato_term_get_text_color(void) {
	return (uint8_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_TEXT_COLOR);
}

inline uint8_t tomato_term_get_background_color(void) {
	return (uint8_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_BACKGROUND_COLOR);
}

inline void tomato_term_set_cursor_pos(uint16_t x, uint16_t y) {
	tomato_syscall_2p(TOMATO_SYSCALL_TERM_SET_CURSOR_POS, (uint32_t)x, (uint32_t)y);
}

inline uint16_t tomato_term_get_cursor_x(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_CURSOR_X);
}

inline uint16_t tomato_term_get_cursor_y(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_WRITE);
}

inline uint16_t tomato_term_get_width(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_WIDTH);
}

inline uint16_t tomato_term_get_height(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_HEIGHT);
}

inline void tomato_term_scroll(uint16_t n) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_SCROLL, (uint32_t)n);
}

inline void tomato_term_clear_line(uint16_t n) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_CLEAR_LINE, (uint32_t)n);
}

#endif