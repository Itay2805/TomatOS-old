#include "term.h"

#include <kernel.h>

#define NATIVE_TERM_VIDEO_ADDRESS 0xb8000

#define NATIVE_TERM_WIDTH 80
#define NATIVE_TERM_HEIGHT 25

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

typedef struct cell {
    uint8_t chr;
    uint8_t fg : 4;
    uint8_t bg : 4;
} PACKED cell_t;

static uint8_t bg_color;
static uint8_t fg_color;
static uint8_t term_x, term_y;
static cell_t* vmemory;

static void update_cursor();

void term_init() {
    vmemory = (cell_t*)NATIVE_TERM_VIDEO_ADDRESS;

    // set default colors
    bg_color = 0;
    fg_color = 0xf;

    // disable blink
    inb(0x03DA);
	outb(0x03C0, 0x30);
	int reg = inb(0x03C1);
	reg &= 0xF7;
	outb(0x03C0, reg);
}

void term_write(const char* text){
    while (*text != 0) {
		char c = *text;
        cell_t* cell = &vmemory[term_x + term_y * NATIVE_TERM_WIDTH];
		if (c == '\n') {
			term_y++;
			term_x = 0;
		}
		else if (c == '\t') {
			cell->chr = ' ';
            cell->bg = bg_color;
			cell->fg = fg_color;
            cell++;
			cell->chr = ' ';
            cell->bg = bg_color;
			cell->fg = fg_color;
			term_x += 2;
		}
		else {
			cell->chr = c;
            cell->bg = bg_color;
			cell->fg = fg_color;
            term_x++;
		}
		text++;

		// go to next line if needed
		if (term_x >= NATIVE_TERM_WIDTH) {
			term_y++;
			term_x = 0;
		}

		// scroll if needed
		if (term_y >= NATIVE_TERM_HEIGHT) {
			term_y--;
			term_x = 0;
			term_scroll(1);
		}
	}
	update_cursor();
}

void term_clear() {
    for (size_t i = 0; i < NATIVE_TERM_HEIGHT * NATIVE_TERM_WIDTH; i++) {
		vmemory[i].chr = 0;
		vmemory[i].bg = bg_color;
        vmemory[i].fg = fg_color;
	}
}

void term_set_text_color(uint8_t color) {
    fg_color = color;
}

void term_set_background_color(uint8_t color) {
    bg_color = color;
}

uint8_t term_get_text_color() {
    return fg_color;
}

uint8_t term_get_background_color() {
    return bg_color;
}

void term_set_cursor_pos(uint8_t x, uint8_t y) {
    if(x >= NATIVE_TERM_WIDTH) term_x = NATIVE_TERM_WIDTH - 1;
    else term_x = x;
    if(y >= NATIVE_TERM_HEIGHT) term_y = NATIVE_TERM_HEIGHT - 1;
    else term_y = y;

	update_cursor();
}

uint8_t term_get_cursor_x() {
    return term_x;
}

uint8_t term_get_cursor_y() {
    return term_y;
}

void term_scroll(uint8_t n) {
    size_t length_copy = ((NATIVE_TERM_HEIGHT - n) * NATIVE_TERM_WIDTH);
	size_t length_remove = (n * NATIVE_TERM_WIDTH);
	for (size_t i = 0; i < length_copy; i++) {
		vmemory[i] = vmemory[i + length_remove];
	}
	for (size_t i = 0; i < length_remove; i++) {
        cell_t* cell = &vmemory[i + length_copy];
		cell->chr = 0;
		cell->bg = bg_color;
        cell->fg = fg_color;
	}
}

uint8_t term_get_height() {
    return NATIVE_TERM_HEIGHT;
}

uint8_t term_get_width() {
    return NATIVE_TERM_WIDTH;
}

void term_clear_line(uint8_t n) {
    size_t offset = (n * NATIVE_TERM_WIDTH);
	for (size_t i = offset; i < NATIVE_TERM_WIDTH + offset; i++) {
		vmemory[i].chr = 0;
		vmemory[i].bg = bg_color;
        vmemory[i].fg = fg_color;
	}
}

static void update_cursor() {
    uint16_t offset = term_x + term_y * NATIVE_TERM_WIDTH;
    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

#undef NATIVE_TERM_VIDEO_ADDRESS
#undef NATIVE_TERM_WIDTH
#undef NATIVE_TERM_HEIGHT
#undef REG_SCREEN_CTRL
#undef REG_SCREEN_DATA