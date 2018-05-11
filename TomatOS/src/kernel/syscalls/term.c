#include "term.h"

#include <stdint.h>
#include <stddef.h>

#include <tomato.h>

#include "../interrupt.h"
#include "../syscalls.h"
#include "../port.h"

static uint8_t     bg_color;
static uint8_t     fg_color;
static uint16_t    term_x, term_y;
static uint8_t*    vmemory;

#define NATIVE_TERM_VIDEO_ADDRESS 0xb8000

#define NATIVE_TERM_WIDTH 80
#define NATIVE_TERM_HEIGHT 25

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/////////////////////////////////////////////////
//// syscalls forward declarations
/////////////////////////////////////////////////

static uint32_t syscall_term_write(registers_t text);
static uint32_t syscall_term_clear(registers_t text);
static uint32_t syscall_term_set_text_color(registers_t text);
static uint32_t syscall_term_set_background_color(registers_t text);
static uint32_t syscall_term_get_text_color(registers_t text);
static uint32_t syscall_term_get_background_color(registers_t text);
static uint32_t syscall_term_set_cursor_pos(registers_t text);
static uint32_t syscall_term_get_cursor_x(registers_t text);
static uint32_t syscall_term_get_cursor_y(registers_t text);
static uint32_t syscall_term_get_width(registers_t text);
static uint32_t syscall_term_get_height(registers_t text);
static uint32_t syscall_term_scroll(registers_t text);
static uint32_t syscall_term_clear_line(registers_t text);

void syscall_term_init(void) {
	// set the video memory buffer
	vmemory = (uint8_t*)NATIVE_TERM_VIDEO_ADDRESS;

	// set default colors
	bg_color = 0;
	fg_color = 0xf;

	// disable blink
	port_read8(0x03DA);
	port_write8(0x03C0, 0x30);
	int reg = port_read8(0x03C1);
	reg &= 0xF7;
	port_write8(0x03C0, reg);

	// register all the syscalls
	register_syscall(TOMATO_SYSCALL_TERM_WRITE, syscall_term_write);
	register_syscall(TOMATO_SYSCALL_TERM_CLEAR, syscall_term_clear);
	register_syscall(TOMATO_SYSCALL_TERM_SET_TEXT_COLOR, syscall_term_set_text_color);
	register_syscall(TOMATO_SYSCALL_TERM_SET_BACKGROUND_COLOR, syscall_term_set_background_color);
	register_syscall(TOMATO_SYSCALL_TERM_GET_TEXT_COLOR, syscall_term_get_text_color);
	register_syscall(TOMATO_SYSCALL_TERM_GET_BACKGROUND_COLOR, syscall_term_get_background_color);
	register_syscall(TOMATO_SYSCALL_TERM_SET_CURSOR_POS, syscall_term_set_cursor_pos);
	register_syscall(TOMATO_SYSCALL_TERM_GET_CURSOR_X, syscall_term_get_cursor_x);
	register_syscall(TOMATO_SYSCALL_TERM_GET_CUSROR_Y, syscall_term_get_cursor_y);
	register_syscall(TOMATO_SYSCALL_TERM_GET_WIDTH, syscall_term_get_width);
	register_syscall(TOMATO_SYSCALL_TERM_GET_HEIGHT, syscall_term_get_height);
	register_syscall(TOMATO_SYSCALL_TERM_SCROLL, syscall_term_scroll);
	register_syscall(TOMATO_SYSCALL_TERM_CLEAR_LINE, syscall_term_clear_line);
}


/////////////////////////////////////////////////
//// helper functions
/////////////////////////////////////////////////

static void update_cursor() {
	int offset = term_x + term_y * NATIVE_TERM_WIDTH;
	port_write8(REG_SCREEN_CTRL, 14);
	port_write8(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_write8(REG_SCREEN_CTRL, 15);
	port_write8(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

static void native_term_scroll(uint16_t n) {
	size_t length_copy = ((NATIVE_TERM_HEIGHT - n) * NATIVE_TERM_WIDTH) * 2;
	size_t length_remove = (n * NATIVE_TERM_WIDTH) * 2;
	for (size_t i = 0; i < length_copy; i++) {
		vmemory[i] = vmemory[i + (n * NATIVE_TERM_WIDTH) * 2];
		vmemory[i + 1] = vmemory[1 + i + (n * NATIVE_TERM_WIDTH) * 2];
	}
	for (size_t i = 0; i < length_remove; i++) {
		vmemory[i + length_copy] = 0;
		vmemory[i + 1 + length_copy] = bg_color << 4 | fg_color;
	}
}

/////////////////////////////////////////////////
//// syscall handlers
/////////////////////////////////////////////////

static uint32_t syscall_term_write(registers_t regs) {
	const char* text = (const char*)regs.ebx;

	while (*text != 0) {
		char c = *text;
		if (c == '\n') {
			term_y++;
			term_x = 0;
		}
		else if (c == '\t') {
			vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 0] = 0;
			vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 1] = bg_color << 4 | fg_color;
			vmemory[2 * (term_x + 1 + term_y * NATIVE_TERM_WIDTH) + 0] = 0;
			vmemory[2 * (term_x + 1 + term_y * NATIVE_TERM_WIDTH) + 1] = bg_color << 4 | fg_color;
			term_x += 2;
		}
		else {
			vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 0] = c;
			vmemory[2 * (term_x + term_y * NATIVE_TERM_WIDTH) + 1] = bg_color << 4 | fg_color;
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
			native_term_scroll(1);
		}
	}
	update_cursor();

	return 0;
}

static uint32_t syscall_term_clear(registers_t regs) {
	UNUSED(regs);
	
	for (size_t i = 0; i < NATIVE_TERM_HEIGHT * NATIVE_TERM_WIDTH * 2; i += 2) {
		vmemory[i + 0] = 0;
		vmemory[i + 1] = bg_color << 4 | fg_color;
	}

	return 0;
}

static uint32_t syscall_term_set_text_color(registers_t regs) {
	uint8_t color = regs.ebx;

	fg_color = color;

	return 0;
}

static uint32_t syscall_term_set_background_color(registers_t regs) {
	uint8_t color = regs.ebx;

	bg_color = color;

	return 0;
}

static uint32_t syscall_term_get_text_color(registers_t regs) {
	UNUSED(regs);

	return fg_color;
}

static uint32_t syscall_term_get_background_color(registers_t regs) {
	UNUSED(regs);

	return bg_color;
}

static uint32_t syscall_term_set_cursor_pos(registers_t regs) {
	uint16_t x = regs.ebx;
	uint16_t y = regs.ecx;

	term_x = x;
	term_y = y;
	update_cursor();

	return 0;
}

static uint32_t syscall_term_get_cursor_x(registers_t regs) {
	UNUSED(regs);

	return term_x;
}

static uint32_t syscall_term_get_cursor_y(registers_t regs) {
	UNUSED(regs);

	return term_y;
}

static uint32_t syscall_term_get_width(registers_t regs) {
	UNUSED(regs);

	return NATIVE_TERM_WIDTH;
}

static uint32_t syscall_term_get_height(registers_t regs) {
	UNUSED(regs);

	return NATIVE_TERM_HEIGHT;
}

static uint32_t syscall_term_scroll(registers_t regs) {
	uint16_t n = (uint16_t)regs.ebx;

	native_term_scroll(n);

	return 0;
}

static uint32_t syscall_term_clear_line(registers_t regs) {
	uint16_t n = (uint16_t)regs.ebx;

	size_t offset = (n * NATIVE_TERM_WIDTH) * 2;
	for (size_t i = offset; i < NATIVE_TERM_WIDTH * 2 + offset; i += 2) {
		vmemory[i + 0] = 0;
		vmemory[i + 1] = bg_color << 4 | fg_color;
	}

	return 0;
}
