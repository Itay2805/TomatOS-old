// for now I am putting this in apis, maybe in the future put this somewhere else

#include <tomato.h>

#include <syscall.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////
//// os syscalls
/////////////////////////////////////////////////////

const char* tomato_os_version() {
	return (const char*)tomato_syscall(TOMATO_SYSCALL_OS_VERSION);
}

/////////////////////////////////////////////////////
//// term syscalls
/////////////////////////////////////////////////////

void tomato_term_write(const char* text) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_WRITE, (uint32_t)text);
}

void tomato_term_clear(void) {
	tomato_syscall(TOMATO_SYSCALL_TERM_CLEAR);
}

void tomato_term_set_text_color(uint8_t color) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_SET_TEXT_COLOR, (uint32_t)color);
}

void tomato_term_set_background_color(uint8_t color) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_SET_BACKGROUND_COLOR, (uint32_t)color);
}

uint8_t tomato_term_get_text_color(void) {
	return (uint8_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_TEXT_COLOR);
}

uint8_t tomato_term_get_background_color(void) {
	return (uint8_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_BACKGROUND_COLOR);
}

void tomato_term_set_cursor_pos(uint16_t x, uint16_t y) {
	tomato_syscall_2p(TOMATO_SYSCALL_TERM_SET_CURSOR_POS, (uint32_t)x, (uint32_t)y);
}

uint16_t tomato_term_get_cursor_x(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_CURSOR_X);
}

uint16_t tomato_term_get_cursor_y(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_WRITE);
}

uint16_t tomato_term_get_width(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_WIDTH);
}

uint16_t tomato_term_get_height(void) {
	return (uint16_t)tomato_syscall(TOMATO_SYSCALL_TERM_GET_HEIGHT);
}

void tomato_term_scroll(uint16_t n) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_SCROLL, (uint32_t)n);
}

void tomato_term_clear_line(uint16_t n) {
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_CLEAR_LINE, (uint32_t)n);
}

/////////////////////////////////////////////////////
//// heap syscalls
/////////////////////////////////////////////////////

void* tomato_heap_allocate(size_t size) {
	return (void*)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_ALLOCATE, (uint32_t)size);
}

void* tomato_heap_reallocate(void* ptr, size_t newsize) {
	return (void*)tomato_syscall_2p(TOMATO_SYSCALL_HEAP_REALLOCATE, (uint32_t)ptr, (uint32_t)newsize);
}

bool_t tomato_heap_free(void* ptr) {
	return (bool_t)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_FREE, (uint32_t)ptr);
}

#ifdef __cplusplus
}
#endif
