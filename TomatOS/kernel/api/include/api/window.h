#ifndef API_WINDOW_H
#define API_WINDOW_H

#include <stdint.h>
#include <stdbool.h>

#include <api/term.h>

typedef struct {
	term_t __term;
	term_t* __parent;
	uint16_t __x, __y;
	uint16_t __width, __height;
	uint16_t __cursor_x, __cursor_y;
	uint8_t __bg_color, __fg_color;
	bool __visible;
	uint8_t* __screenBuffer;
} window_t;

void kernel_init_window(void);

window_t window_create(term_t* parentTerm, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible);
window_t window_destroy(window_t* window);

void window_write(window_t* window, char* text);
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
void window_restore_cursor();
void window_reposition(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

term_t* window_as_term(window_t* window);

#endif