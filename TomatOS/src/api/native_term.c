#include "include/api/term.h"
#include "include/api/color.h"

#include <string.h>
#include <stdbool.h>
#include <memory.h>

#include "../drivers/ports.h"

#define NATIVE_TERM_VIDEO_ADDRESS 0xb8000

#define NATIVE_TERM_WIDTH 80
#define NATIVE_TERM_HEIGHT 25

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

static term_t      native;
static uint8_t     bg_color;
static uint8_t     fg_color;
static uint16_t    term_x, term_y;
static uint8_t*    vmemory;
static bool        cusor_enabled;

static void native_term_write(char* text);
static void native_term_clear(void);
static void native_term_set_text_color(uint8_t color);
static void native_term_set_background_color(uint8_t color);
static uint8_t native_term_get_text_color(void);
static uint8_t native_term_get_background_color(void);
static void native_term_set_cursor_pos(uint16_t x, uint16_t y);
static uint16_t native_term_get_cursor_x(void);
static uint16_t native_term_get_cursor_y(void);
static uint16_t native_term_get_width(void);
static uint16_t native_term_get_height(void);
static void native_term_scroll(uint16_t n);
static void native_term_clear_line(uint16_t n);

static void update_cursor() {
    int offset = term_x + term_y * NATIVE_TERM_WIDTH;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void kernel_init_native_term(void) {
    // set the video memory buffer
    vmemory = (uint8_t*)NATIVE_TERM_VIDEO_ADDRESS;

    // cursor is enabled
    cusor_enabled = true;

    // set default colors
    bg_color = COLOR_BLACK;
    fg_color = COLOR_WHITE;

    // clear screen and reset cursor
	// we can pass nullptr beacuse the native terminal does not use the term_t instance
    native_term_clear();
    native_term_set_cursor_pos(0, 0);

    // set all the function pointers in the native term object
    native.write = native_term_write;
    native.clear = native_term_clear;
    native.set_text_color = native_term_set_text_color;
    native.set_background_color = native_term_set_background_color;
    native.get_text_color = native_term_get_text_color;
    native.get_background_color = native_term_get_background_color;
    native.set_cursor_pos = native_term_set_cursor_pos;
    native.get_cursor_x = native_term_get_cursor_x;
    native.get_cursor_y = native_term_get_cursor_y;
    native.get_width = native_term_get_width;
    native.get_height = native_term_get_height;
    native.scroll = native_term_scroll;
    native.clear_line = native_term_clear_line;

    // redirect the term to the native term
    term_redirect(term_native());
}

static void native_term_write(char* text) {
    while(*text != 0) {
        char c = *text;
        if(c == '\n') {
            term_y++;
            term_x = 0;
        }else if(c == '\t') {
            vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 0]  = 0;
            vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 1]  = bg_color << 4 | fg_color;
            vmemory[2 * (term_x + 1 + term_y * NATIVE_TERM_WIDTH) + 0]  = 0;
            vmemory[2 * (term_x + 1 + term_y * NATIVE_TERM_WIDTH) + 1]  = bg_color << 4 | fg_color;
            term_x += 2;
        }else {
            vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 0]  = c;
            vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 1]  = bg_color << 4 | fg_color;
            term_x++;
        }
        text++;

        // go to next line if needed
        if(term_x >= NATIVE_TERM_WIDTH) {
            term_y++;
			term_x = 0;
		}

        // scroll if needed
        if(term_y >= NATIVE_TERM_HEIGHT) {
            term_y--;
            term_x = 0;
            native_term_scroll(1);
        }
    }
    update_cursor();
}

static void native_term_clear(void) {
    for(size_t i = 0; i < NATIVE_TERM_HEIGHT * NATIVE_TERM_WIDTH * 2; i += 2) {
        vmemory[i + 0] = 0;
        vmemory[i + 1] = bg_color << 4 | fg_color;
    }
}

static void native_term_set_text_color(uint8_t color) {
    fg_color = color;
}

static void native_term_set_background_color(uint8_t color) {
    bg_color = color;
}

static uint8_t native_term_get_text_color(void) {
    return fg_color;
}

static uint8_t native_term_get_background_color(void) {
    return bg_color;
}

static void native_term_set_cursor_pos(uint16_t x, uint16_t y) {
    term_x = x;
    term_y = y;
    update_cursor();
}

static uint16_t native_term_get_cursor_x(void) {
    return term_x;
}

static uint16_t native_term_get_cursor_y(void) {
    return term_y;
}

static uint16_t native_term_get_width(void) {
    return NATIVE_TERM_WIDTH;
}

static uint16_t native_term_get_height(void) {
    return NATIVE_TERM_HEIGHT;
}

static void native_term_scroll(uint16_t n) {
    size_t length_copy = ((NATIVE_TERM_HEIGHT - n) * NATIVE_TERM_WIDTH) * 2;
    size_t length_remove = (n * NATIVE_TERM_WIDTH) * 2;
    for(size_t i = 0; i < length_copy; i++) {
        vmemory[i] = vmemory[i + (n * NATIVE_TERM_WIDTH) * 2];
        vmemory[i + 1] = vmemory[1 + i + (n * NATIVE_TERM_WIDTH) * 2];
    }
    for(size_t i = 0; i < length_remove; i++) {
        vmemory[i + length_copy] = 0;
        vmemory[i + 1 + length_copy] = bg_color << 4 | fg_color;
    }
}

static void native_term_clear_line(uint16_t n) {
    size_t offset = (n * NATIVE_TERM_WIDTH) * 2;
    for(size_t i = offset; i < NATIVE_TERM_WIDTH * 2 + offset; i += 2) {
        vmemory[i + 0] = 0;
        vmemory[i + 1] = bg_color << 4 | fg_color;
    }
}

term_t* term_native(void) {
    return &native;
}

void term_cursor_blink(bool enabled) {
}