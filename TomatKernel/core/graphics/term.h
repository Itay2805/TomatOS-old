#ifndef KERNEL_CORE_GRAPHICS_TERM_H
#define KERNEL_CORE_GRAPHICS_TERM_H

#include <stdint.h>
#include <stdbool.h>

#include <kernel.h>

#include <core/graphics/window.h>

#define COLOR_BLACK				(0x0)
#define COLOR_BLUE				(0x1)
#define COLOR_GREEN				(0x2)
#define COLOR_CYAN				(0x3)
#define COLOR_RED				(0x4)
#define COLOR_MAGENTA			(0x5)
#define COLOR_BROWN				(0x6)
#define COLOR_GRAY				(0x7)
#define COLOR_DARK_GRAY			(0x8)
#define COLOR_LIGHT_BLUE		(0x9)
#define COLOR_LIGHT_GREEN		(0xa)
#define COLOR_LIGHT_CYAN		(0xb)
#define COLOR_LIGHT_RED			(0xc)
#define COLOR_LIGHT_MAGENTA		(0xd)
#define COLOR_YELLOW			(0xe)
#define COLOR_WHITE				(0xf)

#ifdef __cplusplus
extern "C" {
#endif
	
	void term_init();
	void term_register_syscalls();

	void term_write(const char* text);
	void term_clear();
	void term_set_text_color(uint8_t color);
	void term_set_background_color(uint8_t color);
	uint8_t term_get_text_color();
	uint8_t term_get_background_color();
	void term_set_cursor_pos(uint16_t x, uint16_t y);
	uint16_t term_get_cursor_x();
	uint16_t term_get_cursor_y();
	uint16_t term_get_width();
	uint16_t term_get_height();
	void term_scroll(uint16_t n);
	void term_clear_line(uint16_t n);

	window_t* term_redirect(window_t* window);
	window_t* term_current();
	window_t* term_native();

#ifdef __cplusplus
}
#endif

#endif