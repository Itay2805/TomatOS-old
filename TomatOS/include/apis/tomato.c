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

int tomato_os_pull_event(event_t* event, uint32_t filter) {
	return tomato_syscall_2p(TOMATO_SYSCALL_OS_PULL_EVENT, (uint32_t)event, (uint32_t)filter);
}

void tomato_os_queue_event(event_t* event) {
	tomato_syscall_1p(TOMATO_SYSCALL_OS_QUEUE_EVENT, (uint32_t)event);
}

uint32_t tomato_os_start_timer(uint32_t millis) {
	return tomato_syscall_1p(TOMATO_SYSCALL_OS_START_TIMER, millis);
}

void tomato_os_cancel_timer(uint32_t id) {
	tomato_syscall_1p(TOMATO_SYSCALL_OS_CANCEL_TIMER, id);
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

bool tomato_heap_free(void* ptr) {
	return (bool)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_FREE, (uint32_t)ptr);
}

/////////////////////////////////////////////////////
//// fs syscalls
/////////////////////////////////////////////////////

tomato_list_entry_t* tomato_fs_list(const char* path, int* count) {
	return (tomato_list_entry_t*)tomato_syscall_2p(TOMATO_SYSCALL_FS_LIST, path, count);
}

void tomato_fs_list_release(tomato_list_entry_t* entries) {
	tomato_syscall_1p(TOMATO_SYSCALL_FS_LIST_RELEASE, entries);
}

bool tomato_fs_exists(const char* path) {
	return (bool)tomato_syscall_1p(TOMATO_SYSCALL_FS_EXISTS, (uint32_t)path);
}

tomato_file_handle_t* tomato_fs_open(const char* path) {
	return (tomato_file_handle_t*)tomato_syscall_1p(TOMATO_SYSCALL_FS_OPEN, path);
}

void tomato_fs_read_bytes(tomato_file_handle_t* handle, void* buffer, size_t count, size_t offset) {
	tomato_syscall_4p(TOMATO_SYSCALL_FS_READ_BYTES, handle, buffer, count, offset);
}

void tomato_fs_write_bytes(tomato_file_handle_t* handle, void* buffer, size_t count, size_t offset) {
	tomato_syscall_4p(TOMATO_SYSCALL_FS_WRITE_BYTES, handle, buffer, count, offset);
}

void tomato_fs_close(tomato_file_handle_t* handle) {
	tomato_syscall_1p(TOMATO_SYSCALL_FS_CLOSE, handle);
}

void tomato_fs_make_dir(const char* path) {
	tomato_syscall_1p(TOMATO_SYSCALL_FS_MAKE_DIR, path);
}

void tomato_fs_delete(const char* path) {
	tomato_syscall_1p(TOMATO_SYSCALL_FS_DELETE, path);
}


#ifdef __cplusplus
}
#endif
