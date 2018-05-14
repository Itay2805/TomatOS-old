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

#define TOMATO_EVENT_ALL			0x00
#define TOMATO_EVENT_TERMINATE		0x01
#define TOMATO_EVENT_TIMER			0x02
#define TOMATO_EVENT_CHAR			0x04
#define TOMATO_EVENT_KEY			0x08
#define TOMATO_EVENT_KEY_UP			0x16
#define TOMATO_EVENT_EXCEPTION		0x32

	typedef struct event_t {
		uint32_t type;
		size_t data[4];
	} event_t;

	const char* tomato_os_version();
	void tomato_os_pull_event_blocking(event_t* event, uint32_t filter);
	void tomato_os_queue_event(event_t* event);
	uint32_t tomato_os_start_timer(uint32_t millis);
	void tomato_os_cancel_timer(uint32_t id);

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