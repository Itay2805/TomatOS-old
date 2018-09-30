#include <core/graphics/term.h>

#include <string.h>
#include <stdlib.h>
#include <kernel.h>

#include <core/process/syscall.h>

#define NATIVE_TERM_VIDEO_ADDRESS 0xb8000

#define NATIVE_TERM_WIDTH 80
#define NATIVE_TERM_HEIGHT 25

static window_t native;
static window_t* current;

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

// TODO: Should only be accessible to foreground processes!

static void syscall_write(registers_t* regs) {
	// TODO: validate string pointer
	char* text = (char*)regs->ebx;

	term_write(text);
}

static void syscall_clear(registers_t* regs) {
	UNUSED(regs);

	term_clear();
}

static void syscall_set_text_color(registers_t* regs) {
	uint8_t color = regs->ebx;

	term_set_text_color(color);
}

static void syscall_set_background_color(registers_t* regs) {
	uint8_t color = regs->ebx;

	term_set_background_color(color);
}

static void syscall_get_text_color(registers_t* regs) {
	regs->eax = term_get_text_color();
}

static void syscall_get_background_color(registers_t* regs) {
	regs->eax = term_get_background_color();
}

static void syscall_get_cursor_x(registers_t* regs) {
	regs->eax = term_get_cursor_x();
}

static void syscall_get_cursor_y(registers_t* regs) {
	regs->eax = term_get_cursor_y();
}

static void syscall_set_cursor_pos(registers_t* regs) {
	uint16_t x = regs->ebx;
	uint16_t y = regs->ecx;

	term_set_cursor_pos(x, y);
}

static void syscall_scroll(registers_t* regs) {
	uint16_t n = regs->ebx;

	term_scroll(n);
}

static void syscall_width(registers_t* regs) {
	regs->eax = term_get_width();
}

static void syscall_height(registers_t* regs) {
	regs->eax = term_get_height();
}

static void syscall_clear_line(registers_t* regs) {
	uint16_t n = regs->ebx;

	term_clear_line(n);
}

static void syscall_redirect(registers_t* regs) {
	// TODO: validate parent
	window_t* window = (window_t*)regs->ebx;

	regs->eax = (uint32_t)term_redirect(window);
}

static void syscall_native(registers_t* regs) {
	regs->eax = (uint32_t)term_native();
}

static void syscall_current(registers_t* regs) {
	regs->eax = (uint32_t)term_current();
}

void term_register_syscalls() {
	syscall_register(SYSCALL_TERM_WRITE, syscall_write);
	syscall_register(SYSCALL_TERM_CLEAR, syscall_clear);
	syscall_register(SYSCALL_TERM_SET_TEXT_COLOR, syscall_set_text_color);
	syscall_register(SYSCALL_TERM_SET_BACKGROUND_COLOR, syscall_set_background_color);
	syscall_register(SYSCALL_TERM_GET_TEXT_COLOR, syscall_get_text_color);
	syscall_register(SYSCALL_TERM_GET_BACKGROUND_COLOR, syscall_get_background_color);
	syscall_register(SYSCALL_TERM_GET_CURSOR_X, syscall_get_cursor_x);
	syscall_register(SYSCALL_TERM_GET_CURSOR_Y, syscall_get_cursor_y);
	syscall_register(SYSCALL_TERM_SET_CURSOR_POS, syscall_set_cursor_pos);
	syscall_register(SYSCALL_TERM_SCROLL, syscall_scroll);
	syscall_register(SYSCALL_TERM_WIDTH, syscall_width);
	syscall_register(SYSCALL_TERM_HEIGHT, syscall_height);
	syscall_register(SYSCALL_TERM_CLEAR_LINE, syscall_clear_line);

	syscall_register(SYSCALL_TERM_REDIRECT, syscall_redirect);
	syscall_register(SYSCALL_TERM_NATIVE, syscall_native);
	syscall_register(SYSCALL_TERM_CURRENT, syscall_current);
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

	current = &native;
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
