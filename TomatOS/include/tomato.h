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
	//// keys
	/////////////////////////////////////////////////////

#define TOMATO_KEYS_SHIFT_LEFT		160
#define TOMATO_KEYS_SHIFT_RIGHT		161

#define TOMATO_KEYS_NUMPAD0			96
#define TOMATO_KEYS_NUMPAD1			97
#define TOMATO_KEYS_NUMPAD2			98
#define TOMATO_KEYS_NUMPAD3			99
#define TOMATO_KEYS_NUMPAD4			100
#define TOMATO_KEYS_NUMPAD5			101
#define TOMATO_KEYS_NUMPAD6			102
#define TOMATO_KEYS_NUMPAD7			103
#define TOMATO_KEYS_NUMPAD8			104
#define TOMATO_KEYS_NUMPAD9			105

	/////////////////////////////////////////////////////
	//// os syscalls
	/////////////////////////////////////////////////////

#define TOMATO_EVENT_ALL			0
#define TOMATO_EVENT_TERMINATE		1
#define TOMATO_EVENT_TIMER			2
#define TOMATO_EVENT_CHAR			3
#define TOMATO_EVENT_KEY			4
#define TOMATO_EVENT_KEY_UP			5
#define TOMATO_EVENT_EXCEPTION		6

	typedef struct event_t {
		uint32_t type;
		size_t data[4];
	} event_t;

	const char* tomato_os_version();
	int tomato_os_pull_event(event_t* event, uint32_t filter);
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
	bool tomato_heap_free(void* ptr);

	/////////////////////////////////////////////////////
	//// fs syscalls
	/////////////////////////////////////////////////////

#define TOMATO_FS_NOT_EXISTS	0
#define TOMATO_FS_FILE			0x454c4946
#define TOMATO_FS_FOLDER		0x52444c46

	typedef struct tomato_file_handle_t {
		char* name;
		size_t size;
		uint32_t type;
	} tomato_file_handle_t;

	typedef struct tomato_list_entry_t {
		char* name;
	} tomato_list_entry_t;

	tomato_list_entry_t* tomato_fs_list(const char* path, int* count);
	void tomato_fs_list_release(tomato_list_entry_t* entries);
	bool tomato_fs_exists(const char* path);
	tomato_file_handle_t* tomato_fs_open(const char* path);
	void tomato_fs_read_bytes(tomato_file_handle_t* handle, void* buffer, size_t count, size_t offset);
	void tomato_fs_write_bytes(tomato_file_handle_t* handle, void* buffer, size_t count, size_t offset);
	void tomato_fs_close(tomato_file_handle_t* handle);
	void tomato_fs_make_dir(const char* path);
	void tomato_fs_delete(const char* path);

#ifdef __cplusplus
}
#endif

#endif