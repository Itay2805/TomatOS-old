#include <core/graphics/term.h>

#include <string.h>
#include <stdlib.h>

#define NATIVE_TERM_VIDEO_ADDRESS 0xb8000

#define NATIVE_TERM_WIDTH 80
#define NATIVE_TERM_HEIGHT 25

static window_t native;
static window_t* current;

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

void term_register_syscalls() {
	// TODO: register syscalls...
}

//////////////////////////////////////////////////////////////
//// implementation
//////////////////////////////////////////////////////////////

void term_init() {
	native.width = NATIVE_TERM_WIDTH;
	native.height = NATIVE_TERM_HEIGHT;

	// we will set this to false just because no redraw is needed technically 
	native.visible = false;

	native.parent = 0;
	native.x = 0;
	native.y = 0;
	native.screen_buffer = (cell_t*)NATIVE_TERM_VIDEO_ADDRESS;
	native.cursor_x = 0;
	native.cursor_y = 0;
	native.bg_col = 0x0;
	native.fg_col = 0xf;
}

void term_write(const char* text) {
	window_write(current, text);
}

void term_clear() {
	window_clear(current);
}

void term_set_text_color(uint8_t color) {
	window_set_text_color(current, color);
}

void term_set_background_color(uint8_t color) {
	window_set_background_color(current, color);
}

uint8_t term_get_text_color() {
	return window_get_text_color(current);
}

uint8_t term_get_background_color() {
	return window_get_background_color(current);
}

void term_set_cursor_pos(uint16_t x, uint16_t y) {
	window_set_cursor_pos(current, x, y);
}

uint16_t term_get_cursor_x() {
	return window_get_cursor_x(current);
}

uint16_t term_get_cursor_y() {
	return window_get_cursor_y(current);
}

uint16_t term_get_width() {
	return window_get_width(current);
}

uint16_t term_get_height() {
	return window_get_height(current);
}

void term_scroll(uint16_t n) {
	window_scroll(current, n);
}

void term_clear_line(uint16_t n) {
	window_clear_line(current, n);
}

window_t* term_redirect(window_t* window) {
	window_t* temp = current;
	current = window;
	return temp;
}

window_t* term_current() {
	return current;
}

window_t* term_native() {
	return &native;
}
