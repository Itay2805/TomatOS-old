#ifndef KERNEL_CORE_GRAPHICS_WINDOW_H
#define KERNEL_CORE_GRAPHICS_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#include <kernel.h>

#define WINDOW_MAGIC ((uint32_t)(0xDEADBEEF))
#define IS_WINDOW(window) (IS_KERNEL_POINTER(window) && window->magic == WINDOW_MAGIC) 

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cell {
		uint8_t chr;
		uint8_t fg : 4;
		uint8_t bg : 4;
	} PACKED cell_t;

	typedef struct window {
		uint32_t magic;

		struct window* parent;

		uint32_t uid;

		uint16_t x, y;
		uint16_t width, height;
		uint16_t cursor_x, cursor_y;
		uint8_t bg_col, fg_col;
		bool visible;
		cell_t* screen_buffer;
	} window_t;

	void window_register_syscalls();

	window_t* window_create(window_t* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible);
	void window_delete(window_t* window);
	void window_delete_for_process(uint32_t uid);
	
	void window_write(window_t* window, const char* text);
	void window_clear(window_t* window);
	void window_set_text_color(window_t* window, uint8_t color);
	void window_set_background_color(window_t* window, uint8_t color);
	uint8_t window_get_text_color(window_t* window);
	uint8_t window_get_background_color(window_t* window);
	void window_set_cursor_pos(window_t* window, uint16_t x, uint16_t y);
	uint16_t window_get_cursor_x(window_t* window);
	uint16_t window_get_cursor_y(window_t* window);
	uint16_t window_get_width(window_t* window);
	uint16_t window_get_height(window_t* window);
	void window_scroll(window_t* window, uint16_t n);
	void window_clear_line(window_t* window, uint16_t n);

	void window_set_visible(window_t* window, bool vis);
	void window_redraw(window_t* window);
	uint16_t window_get_x(window_t* window);
	uint16_t window_get_y(window_t* window);
	void window_restore_cursor(window_t* window);
	void window_reposition(window_t* window, uint16_t x, uint16_t y, uint16_t width, uint16_t height);


#ifdef __cplusplus
}
#endif

#endif 