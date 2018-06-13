#include "term.h"

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

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

static void syscall_term_write(registers_t* text);
static void syscall_term_clear(registers_t* text);
static void syscall_term_set_text_color(registers_t* text);
static void syscall_term_set_background_color(registers_t* text);
static void syscall_term_get_text_color(registers_t* text);
static void syscall_term_get_background_color(registers_t* text);
static void syscall_term_set_cursor_pos(registers_t* text);
static void syscall_term_get_cursor_x(registers_t* text);
static void syscall_term_get_cursor_y(registers_t* text);
static void syscall_term_get_width(registers_t* text);
static void syscall_term_get_height(registers_t* text);
static void syscall_term_scroll(registers_t* text);
static void syscall_term_clear_line(registers_t* text);

void syscall_term_init(void) {
	// set the video memory buffer
	vmemory = (uint8_t*)NATIVE_TERM_VIDEO_ADDRESS;

	// set default colors
	bg_color = 0;
	fg_color = 0xf;

	// disable blink
	inb(0x03DA);
	outb(0x03C0, 0x30);
	int reg = inb(0x03C1);
	reg &= 0xF7;
	outb(0x03C0, reg);

	// register all the syscalls
	term_kwrite("syscall init: registering TERM syscalls\n");
	register_syscall(TOMATO_SYSCALL_TERM_WRITE, syscall_term_write);
	register_syscall(TOMATO_SYSCALL_TERM_CLEAR, syscall_term_clear);
	register_syscall(TOMATO_SYSCALL_TERM_SET_TEXT_COLOR, syscall_term_set_text_color);
	register_syscall(TOMATO_SYSCALL_TERM_SET_BACKGROUND_COLOR, syscall_term_set_background_color);
	register_syscall(TOMATO_SYSCALL_TERM_GET_TEXT_COLOR, syscall_term_get_text_color);
	register_syscall(TOMATO_SYSCALL_TERM_GET_BACKGROUND_COLOR, syscall_term_get_background_color);
	register_syscall(TOMATO_SYSCALL_TERM_SET_CURSOR_POS, syscall_term_set_cursor_pos);
	register_syscall(TOMATO_SYSCALL_TERM_GET_CURSOR_X, syscall_term_get_cursor_x);
	register_syscall(TOMATO_SYSCALL_TERM_GET_CURSOR_Y, syscall_term_get_cursor_y);
	register_syscall(TOMATO_SYSCALL_TERM_GET_WIDTH, syscall_term_get_width);
	register_syscall(TOMATO_SYSCALL_TERM_GET_HEIGHT, syscall_term_get_height);
	register_syscall(TOMATO_SYSCALL_TERM_SCROLL, syscall_term_scroll);
	register_syscall(TOMATO_SYSCALL_TERM_CLEAR_LINE, syscall_term_clear_line);
}

void term_kreset(void) {
	term_x = 0;
	term_y = 0;
}

void term_kwrite_int(int number) {
	char buf[11];
	itoa(number, buf, 10);
	term_kwrite(buf);
}

void kprintf(char* format, ...) {
	va_list arg;
	va_start(arg, format);
	char charbuf[2];
	charbuf[1] = 0;

	char numbuf[20];

	int count = 0;
	bool specifier = false;
	while (*format) {
		if (specifier) {
			switch (*format) {
			case 'x': {
				uint32_t num = va_arg(arg, uint32_t);
				itoa(num, numbuf, 16);
				count += strlen(numbuf);
				tomato_term_write(numbuf);
			} break;
			case 'i':
			case 'd': {
				int32_t num = va_arg(arg, int32_t);
				itoa(num, numbuf, 10);
				count += strlen(numbuf);
				tomato_term_write(numbuf);
			} break;
			case 's': {
				char* str = va_arg(arg, char*);
				count += strlen(str);
				tomato_term_write(str);
			} break;
			case 'p': {
				uint32_t ptr = va_arg(arg, uint32_t);
				itoa(ptr, numbuf, 16);
				count += strlen(numbuf);
				tomato_term_write(numbuf);
			} break;
			case '%': {
				charbuf[0] = '%';
				tomato_term_write(charbuf);
				count++;
			} break;
			}
			specifier = false;
		}
		else {
			if (*format == '%') {
				specifier = true;
			}
			else {
				charbuf[0] = *format;
				tomato_term_write(charbuf);
				count++;
			}
		}
		format++;
	}

	va_end(arg);
	return count;
}

/////////////////////////////////////////////////
//// helper functions
/////////////////////////////////////////////////

static void update_cursor() {
	int offset = term_x + term_y * NATIVE_TERM_WIDTH;
	outb(REG_SCREEN_CTRL, 14);
	outb(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	outb(REG_SCREEN_CTRL, 15);
	outb(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

static void native_term_scroll(uint16_t n) {
	size_t length_copy = ((NATIVE_TERM_HEIGHT - n) * NATIVE_TERM_WIDTH) * 2;
	size_t length_remove = (n * NATIVE_TERM_WIDTH) * 2;
	for (size_t i = 0; i < length_copy; i++) {
		vmemory[i] = vmemory[i + length_remove];
	}
	for (int i = 0; i < length_remove; i += 2) {
		vmemory[i + length_copy] = 0;
		vmemory[i + 1 + length_copy] = bg_color << 4 | fg_color;
	}
}

/////////////////////////////////////////////////
//// syscall handlers
/////////////////////////////////////////////////

void term_kwrite(const char* text) {
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
}

static void syscall_term_write(registers_t* regs) {
	const char* text = (const char*)regs->ebx;

	term_kwrite(text);
}

static void syscall_term_clear(registers_t* regs) {
	UNUSED(regs);
	
	for (size_t i = 0; i < NATIVE_TERM_HEIGHT * NATIVE_TERM_WIDTH * 2; i += 2) {
		vmemory[i + 0] = 0;
		vmemory[i + 1] = bg_color << 4 | fg_color;
	}
}

static void syscall_term_set_text_color(registers_t* regs) {
	uint8_t color = regs->ebx;

	fg_color = color;
}

static void syscall_term_set_background_color(registers_t* regs) {
	uint8_t color = regs->ebx;

	bg_color = color;
}

static void syscall_term_get_text_color(registers_t* regs) {
	regs->eax = fg_color;
}

static void syscall_term_get_background_color(registers_t* regs) {
	regs->eax = bg_color;
}

static void syscall_term_set_cursor_pos(registers_t* regs) {
	uint16_t x = regs->ebx;
	uint16_t y = regs->ecx;

	term_x = x;
	term_y = y;
	update_cursor();
}

static void syscall_term_get_cursor_x(registers_t* regs) {
	regs->eax = term_x;
}

static void syscall_term_get_cursor_y(registers_t* regs) {
	regs->eax = term_y;
}

static void syscall_term_get_width(registers_t* regs) {
	regs->eax = NATIVE_TERM_WIDTH;
}

static void syscall_term_get_height(registers_t* regs) {
	UNUSED(regs);
	regs->eax = NATIVE_TERM_HEIGHT;
}

static void syscall_term_scroll(registers_t* regs) {
	uint16_t n = (uint16_t)regs->ebx;

	native_term_scroll(n);
}

static void syscall_term_clear_line(registers_t* regs) {
	uint16_t n = (uint16_t)regs->ebx;



	size_t offset = (n * NATIVE_TERM_WIDTH) * 2;
	for (size_t i = offset; i < NATIVE_TERM_WIDTH * 2 + offset; i += 2) {
		vmemory[i + 0] = 0;
		vmemory[i + 1] = bg_color << 4 | fg_color;
	}
}
