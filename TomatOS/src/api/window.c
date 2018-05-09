#include "include/api/window.h"

#include <memory.h>
#include <string.h>

#include <api/color.h>

#define MIN(a, b) ((a) > (b)) ? (b) : (a)

///////////////////////////////////////////////////////
/// Terminal Window setup
///////////////////////////////////////////////////////

static term_t window_term;

static void window_term_write(char* text);
static void window_term_clear(void);
static void window_term_set_text_color(uint8_t color);
static void window_term_set_background_color(uint8_t color);
static uint8_t window_term_get_text_color(void);
static uint8_t window_term_get_background_color(void);
static void window_term_set_cursor_pos(uint16_t x, uint16_t y);
static uint16_t window_term_get_cursor_x(void);
static uint16_t window_term_get_cursor_y(void);
static uint16_t window_term_get_width(void);
static uint16_t window_term_get_height(void);
static void window_term_scroll(uint16_t n);
static void window_term_clear_line(uint16_t n);

void kernel_init_window(void) {
	window_term.write = window_term_write;
	window_term.clear = window_term_clear;
	window_term.set_text_color = window_term_set_text_color;
	window_term.set_background_color = window_term_set_background_color;
	window_term.get_text_color = window_term_get_text_color;
	window_term.get_background_color = window_term_get_background_color;
	window_term.set_cursor_pos = window_term_set_cursor_pos;
	window_term.get_cursor_x = window_term_get_cursor_x;
	window_term.get_cursor_y = window_term_get_cursor_y;
	window_term.get_width = window_term_get_width;
	window_term.get_height = window_term_get_height;
	window_term.scroll = window_term_scroll;
	window_term.clear_line = window_term_clear_line;
}

///////////////////////////////////////////////////////
/// Window implementation
///////////////////////////////////////////////////////

window_t window_create(term_t* parentTerm, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible) {
	window_t window;

	window.__term = window_term;

	window.__parent = parentTerm;
	window.__x = x;
	window.__y = y;
	window.__width = width;
	window.__height = height;
	window.__visible = visible;
	window.__cursor_x = 0;
	window.__cursor_y = 0;
	window.__bg_color = COLOR_BLACK;
	window.__fg_color = COLOR_WHITE;
	window.__screenBuffer = malloc(width * height * 2);
	
	return window;
}


void window_destroy(window_t* window) {
	free(window->__screenBuffer);
	window->__screenBuffer = nullptr;
}

void window_write(window_t* window, char* text) {
	while (*text != 0) {
		char c = *text;
		if (c == '\n') {
			window->__cursor_y++;
			window->__cursor_x = 0;
		}
		else if (c == '\t') {
			window->__screenBuffer[2 * (window->__cursor_x + window->__cursor_y * window->__width) + 0] = 0;
			window->__screenBuffer[2 * (window->__cursor_x + window->__cursor_y * window->__width) + 1] = (uint8_t)(window->__bg_color << 4 | window->__fg_color);
			window->__screenBuffer[2 * (window->__cursor_x + 1 + window->__cursor_y * window->__width) + 0] = 0;
			window->__screenBuffer[2 * (window->__cursor_x + 1 + window->__cursor_y * window->__width) + 1] = (uint8_t)(window->__bg_color << 4 | window->__fg_color);
			window->__cursor_x += 2;
		}
		else {
			window->__screenBuffer[2 * (window->__cursor_x + window->__cursor_y * window->__width) + 0] = c;
			window->__screenBuffer[2 * (window->__cursor_x + window->__cursor_y * window->__width) + 1] = (uint8_t)(window->__bg_color << 4 | window->__fg_color);
			window->__cursor_x++;
		}
		text++;

		// go to next line if needed
		if (window->__cursor_x >= window->__width) {
			window->__cursor_y++;
			window->__cursor_x = 0;
		}

		// scroll if needed
		if (window->__cursor_y >= window->__height) {
			window->__cursor_y--;
			window->__cursor_x = 0;
			window_scroll(window, 1);
		}
	}
	if (window->__visible) {
		window_redraw(window);
	}
}

void window_clear(window_t* window) {
	for (int32_t i = 0; i < window->__height * window->__width * 2; i += 2) {
		window->__screenBuffer[i + 0] = 0;
		window->__screenBuffer[i + 1] = (uint8_t)(window->__bg_color << 4 | window->__fg_color);
	}
	if (window->__visible) {
		window_redraw(window);
	}
}

void window_set_text_color(window_t* window, uint8_t color) {
	window->__fg_color = color;
}

void window_set_background_color(window_t* window, uint8_t color) {
	window->__bg_color = color;
}

uint8_t window_get_text_color(window_t* window) {
	return window->__fg_color;
}

uint8_t window_get_background_color(window_t* window) {
	return window->__bg_color;
}

void window_set_cursor_pos(window_t* window, uint16_t x, uint16_t y) {
	window->__cursor_x = x;
	window->__cursor_y = y;
}

uint16_t window_get_cursor_x(window_t* window) {
	return window->__cursor_x;
}

uint16_t window_get_cursor_y(window_t* window) {
	return window->__cursor_y;
}

uint16_t window_get_width(window_t* window) {
	return window->__width;
}

uint16_t window_get_height(window_t* window) {
	return window->__height;
}

void window_scroll(window_t* window, uint16_t n) {
	int32_t length_copy = ((window->__height - n) * window->__width) * 2;
	int32_t length_remove = (n * window->__width) * 2;
	for (int32_t i = 0; i < length_copy; i++) {
		window->__screenBuffer[i] = window->__screenBuffer[i + (n * window->__width) * 2];
		window->__screenBuffer[i + 1] = window->__screenBuffer[1 + i + (n * window->__width) * 2];
	}
	for (int32_t i = 0; i < length_remove; i++) {
		window->__screenBuffer[i + length_copy] = 0;
		window->__screenBuffer[i + 1 + length_copy] = (uint8_t)(window->__bg_color << 4 | window->__fg_color);
	}
	if (window->__visible) {
		window_redraw(window);
	}
}

void window_clear_line(window_t* window, uint16_t n) {
	int32_t offset = (n * window->__width) * 2;
	for (int32_t i = offset; i < window->__width * 2 + offset; i += 2) {
		window->__screenBuffer[i + 0] = 0;
		window->__screenBuffer[i + 1] = (uint8_t)(window->__bg_color << 4 | window->__fg_color);
	}
	if (window->__visible) {
		window_redraw(window);
	}
}

term_t* window_as_term(window_t* window) {
	// because we have all the bindings in the start we can just cast it
	return (term_t*)window;
}

void window_set_visible(window_t* window, bool vis) {
	window->__visible = vis;
	if (vis) {
		window_redraw(window);
	}
}

void window_restore_cursor(window_t* window) {
	uint16_t newX = MIN(window->__x + window->__cursor_x, window->__parent->get_width() - 1);
	uint16_t newY = MIN(window->__y + window->__cursor_y, window->__parent->get_height() - 1);
	window->__parent->set_cursor_pos(newX, newY);
}

uint16_t window_get_x(window_t* window) {
	return window->__x;
}

uint16_t window_get_y(window_t* window) {
	return window->__y;
}

void window_reposition(window_t* window, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	// allocate new buffer
	uint8_t* newScreenBuffer = malloc(width * height * 2);
	
	uint16_t copyWidth = MIN(width, window->__width);
	uint16_t copyHeight = MIN(height, window->__height);

	for (uint16_t cy = 0; cy < copyHeight; cy++) {
		for (uint16_t cx = 0; cx < copyWidth; cx++) {
			newScreenBuffer[(cx + cy * width) * 2] = window->__screenBuffer[(cx + cy * window->__width) * 2];
			newScreenBuffer[(cx + cy * width) * 2 + 1] = window->__screenBuffer[(cx + cy * window->__width) * 2 + 1];
		}
	}

	free(window->__screenBuffer);
	
	window->__screenBuffer = newScreenBuffer;
	window->__width = width;
	window->__height = height;
	window->__x = x;
	window->__y = y;

	if (window->__visible) {
		window_redraw(window);
	}
}

// do this in a more efficient way
// maybe some kind of a blit function?
void window_redraw(window_t* window) {
	if (!window->__visible) return;

	term_t* parent = window->__parent;
	
	//uint16_t savedX = parent->get_cursor_x();
	//uint16_t savedY = parent->get_cursor_y();
	uint8_t savedTextColor = parent->get_text_color();
	uint8_t savedBackgroundColor = parent->get_background_color();

	uint16_t width = MIN(window->__width, parent->get_width() - window->__width + window->__x);
	uint16_t height = MIN(window->__height, parent->get_height() - window->__height + window->__y);
	
	// we need something better for this :think:
	char buff[2];
	buff[1] = 0;
	for (uint16_t y = 0; y < height; y++) {

		// prepare the next row
		parent->set_cursor_pos(window->__x, (uint16_t)(window->__y + y));
		for (uint16_t x = 0; x < width; x++) {
			char c = (char)window->__screenBuffer[(x + y * window->__width) * 2];
			if (c == 0) c = ' ';
			buff[0] = c;
			uint8_t color = window->__screenBuffer[(x + y * window->__width) * 2 + 1];
			uint8_t fg = color & 0xF;
			uint8_t bg = (color >> 4) & 0xF;
			
			parent->set_text_color(fg);
			parent->set_background_color(bg);
			parent->write(buff);
		}
	}
	window_restore_cursor(window);
	// parent->set_cursor_pos(savedX, savedY);
	parent->set_text_color(savedTextColor);
	parent->set_background_color(savedBackgroundColor);
}

///////////////////////////////////////////////////////
/// Terminal Window implementation
///////////////////////////////////////////////////////

#define API_WINDOW_CURRENT_WINDOW ((window_t*)term_current())

static void window_term_write(char* text) {
	window_write(API_WINDOW_CURRENT_WINDOW, text);
}

static void window_term_clear(void) {
	window_clear(API_WINDOW_CURRENT_WINDOW);
}

static void window_term_set_text_color(uint8_t color) {
	window_set_text_color(API_WINDOW_CURRENT_WINDOW, color);
}

static void window_term_set_background_color(uint8_t color) {
	window_set_background_color(API_WINDOW_CURRENT_WINDOW, color);
}

static uint8_t window_term_get_text_color(void) {
	return window_get_text_color(API_WINDOW_CURRENT_WINDOW);
}

static uint8_t window_term_get_background_color(void) {
	return window_get_background_color(API_WINDOW_CURRENT_WINDOW);
}

static void window_term_set_cursor_pos(uint16_t x, uint16_t y) {
	window_set_cursor_pos(API_WINDOW_CURRENT_WINDOW, x, y);
}

static uint16_t window_term_get_cursor_x(void) {
	return window_get_cursor_x(API_WINDOW_CURRENT_WINDOW);
}

static uint16_t window_term_get_cursor_y(void) {
	return window_get_cursor_y(API_WINDOW_CURRENT_WINDOW);
}

static uint16_t window_term_get_width(void) {
	return window_get_width(API_WINDOW_CURRENT_WINDOW);
}

static uint16_t window_term_get_height(void) {
	return window_get_height(API_WINDOW_CURRENT_WINDOW);
}

static void window_term_scroll(uint16_t n) {
	window_scroll(API_WINDOW_CURRENT_WINDOW, n);
}

static void window_term_clear_line(uint16_t n) {
	window_clear_line(API_WINDOW_CURRENT_WINDOW, n);
}

#undef API_WINDOW_CURRENT_WINDOW
