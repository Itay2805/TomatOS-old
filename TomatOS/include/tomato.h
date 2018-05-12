#ifndef TOMATO_H
#define TOMATO_H

// this will contain all the system calls
// for C++ headers look at the apis include dir

#include <syscall.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

	/////////////////////////////////////////////////////
	//// os syscalls
	/////////////////////////////////////////////////////

	const char* tomato_os_version();

	/////////////////////////////////////////////////////
	//// term syscalls
	/////////////////////////////////////////////////////

	void tomato_term_write(const char* text);
	void tomato_term_clear(void);
	void tomato_term_set_text_color(uint8_t color);
	void tomato_term_set_background_color(uint8_t color);
	uint8_t tomato_term_get_text_color(void);
	uint8_t tomato_term_get_background_color(void);
	void tomato_term_set_cursor_pos(uint16_t x, uint16_t y);
	uint16_t tomato_term_get_cursor_x(void);
	uint16_t tomato_term_get_cursor_y(void);
	uint16_t tomato_term_get_width(void);
	uint16_t tomato_term_get_height(void);
	void tomato_term_scroll(uint16_t n);
	void tomato_term_clear_line(uint16_t n);

	/////////////////////////////////////////////////////
	//// heap syscalls
	/////////////////////////////////////////////////////

	void* tomato_heap_allocate(size_t size);
	void* tomato_heap_reallocate(void* ptr, size_t newsize);
	bool_t tomato_heap_free(void* ptr);

#ifdef __cplusplus
}
#endif

#endif