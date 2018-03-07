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

term_t      __native;
uint8_t     __bg_color;
uint8_t     __fg_color;
uint16_t    __term_x, __term_y;
uint8_t*    __vmemory;
bool        __cusor_enabled;

void native_term_write(char* text);
void native_term_clear(void);
void native_term_set_text_color(uint8_t color);
void native_term_set_background_color(uint8_t color);
uint8_t native_term_get_text_color(void);
uint8_t native_term_get_background_color(void);
void native_term_set_cursor_pos(uint16_t x, uint16_t y);
uint16_t native_term_get_cursor_x(void);
uint16_t native_term_get_cursor_y(void);
uint16_t native_term_get_width(void);
uint16_t native_term_get_height(void);
void native_term_scroll(uint16_t n);
void native_term_clear_line(uint16_t n);

void __update_cursor() {
    int offset = __term_x + __term_y * NATIVE_TERM_WIDTH;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void kernel_init_native_term(void) {
    // set the video memory buffer
    __vmemory = (uint8_t*)NATIVE_TERM_VIDEO_ADDRESS;

    // cursor is enabled
    __cusor_enabled = true;

    // set default colors
    __bg_color = COLOR_BLACK;
    __fg_color = COLOR_WHITE;

    // clear screen and reset cursor
	// we can pass nullptr beacuse the native terminal does not use the term_t instance
    native_term_clear();
    native_term_set_cursor_pos(0, 0);

    // set all the function pointers in the native term object
    __native.write = native_term_write;
    __native.clear = native_term_clear;
    __native.set_text_color = native_term_set_text_color;
    __native.set_background_color = native_term_set_background_color;
    __native.get_text_color = native_term_get_text_color;
    __native.get_background_color = native_term_get_background_color;
    __native.set_cursor_pos = native_term_set_cursor_pos;
    __native.get_cursor_x = native_term_get_cursor_x;
    __native.get_cursor_y = native_term_get_cursor_y;
    __native.get_width = native_term_get_width;
    __native.get_height = native_term_get_height;
    __native.scroll = native_term_scroll;
    __native.clear_line = native_term_clear_line;

    // redirect the term to the native term
    term_redirect(term_native());
}

void native_term_write(char* text) {
    while(*text != 0) {
        char c = *text;
        if(c == '\n') {
            __term_y++;
            __term_x = 0;
        }else if(c == '\t') {
            __vmemory[2 * (__term_x + __term_y * NATIVE_TERM_WIDTH) + 0]  = 0;
            __vmemory[2 * (__term_x + __term_y * NATIVE_TERM_WIDTH) + 1]  = __bg_color << 4 | __fg_color;
            __vmemory[2 * (__term_x + 1 + __term_y * NATIVE_TERM_WIDTH) + 0]  = 0;
            __vmemory[2 * (__term_x + 1 + __term_y * NATIVE_TERM_WIDTH) + 1]  = __bg_color << 4 | __fg_color;
            __term_x += 2;
        }else {
            __vmemory[2 * (__term_x + __term_y * NATIVE_TERM_WIDTH) + 0]  = c;
            __vmemory[2 * (__term_x + __term_y * NATIVE_TERM_WIDTH) + 1]  = __bg_color << 4 | __fg_color;
            __term_x++;
        }
        text++;

        // go to next line if needed
        if(__term_x >= NATIVE_TERM_WIDTH) {
            __term_y++;
			__term_x = 0;
		}

        // scroll if needed
        if(__term_y >= NATIVE_TERM_HEIGHT) {
            __term_y--;
            __term_x = 0;
            native_term_scroll(1);
        }
    }
    __update_cursor();
}

void native_term_clear(void) {
    for(size_t i = 0; i < NATIVE_TERM_HEIGHT * NATIVE_TERM_WIDTH * 2; i += 2) {
        __vmemory[i + 0] = 0;
        __vmemory[i + 1] = __bg_color << 4 | __fg_color;
    }
}

void native_term_set_text_color(uint8_t color) {
    __fg_color = color;
}

void native_term_set_background_color(uint8_t color) {
    __bg_color = color;
}

uint8_t native_term_get_text_color(void) {
    return __fg_color;
}

uint8_t native_term_get_background_color(void) {
    return __bg_color;
}

void native_term_set_cursor_pos(uint16_t x, uint16_t y) {
    __term_x = x;
    __term_y = y;
    __update_cursor();
}

uint16_t native_term_get_cursor_x(void) {
    return __term_x;
}

uint16_t native_term_get_cursor_y(void) {
    return __term_y;
}

uint16_t native_term_get_width(void) {
    return NATIVE_TERM_WIDTH;
}

uint16_t native_term_get_height(void) {
    return NATIVE_TERM_HEIGHT;
}

void native_term_scroll(uint16_t n) {
    size_t length_copy = ((NATIVE_TERM_HEIGHT - n) * NATIVE_TERM_WIDTH) * 2;
    size_t length_remove = (n * NATIVE_TERM_WIDTH) * 2;
    for(size_t i = 0; i < length_copy; i++) {
        __vmemory[i] = __vmemory[i + (n * NATIVE_TERM_WIDTH) * 2];
        __vmemory[i + 1] = __vmemory[1 + i + (n * NATIVE_TERM_WIDTH) * 2];
    }
    for(size_t i = 0; i < length_remove; i++) {
        __vmemory[i + length_copy] = 0;
        __vmemory[i + 1 + length_copy] = __bg_color << 4 | __fg_color;
    }
}

void native_term_clear_line(uint16_t n) {
    size_t offset = (n * NATIVE_TERM_WIDTH) * 2;
    for(size_t i = offset; i < NATIVE_TERM_WIDTH * 2 + offset; i += 2) {
        __vmemory[i + 0] = 0;
        __vmemory[i + 1] = __bg_color << 4 | __fg_color;
    }
}

term_t* term_native(void) {
    return &__native;
}

void term_cursor_blink(bool enabled) {
}