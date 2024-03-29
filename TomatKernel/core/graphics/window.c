#include "window.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <core/process/syscall.h>
#include <core/process/process.h>
#include <core/process/perm.h>

#include <core/graphics/term.h>

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

// TODO: Should only be accessible to foreground processes!

static void syscall_create(registers_t* regs) {
	window_t* parent = (window_t*)regs->ebx;

	uint16_t x = regs->ecx;
	uint16_t y = regs->edx;
	uint16_t width = regs->esi;
	uint16_t height = regs->edi;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	window_t* window = window_create(parent, x, y, width, height, true);
	window->uid = process->uid;

	regs->eax = (uint32_t)window;
}

static void syscall_delete(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_delete(window);
}

static void syscall_write(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	char* text = (char*)regs->ecx;
	
	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_write(window, text);
}

static void syscall_clear(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_clear(window);
}

static void syscall_set_text_color(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	uint8_t color = regs->ecx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_set_text_color(window, color);
}

static void syscall_set_background_color(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	uint8_t color = regs->ecx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_set_background_color(window, color);
}

static void syscall_get_text_color(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_text_color(window);
}

static void syscall_get_background_color(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_background_color(window);
}

static void syscall_get_cursor_x(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_cursor_x(window);
}

static void syscall_get_cursor_y(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_cursor_y(window);
}

static void syscall_set_cursor_pos(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	uint16_t x = regs->ecx;
	uint16_t y = regs->edx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_set_cursor_pos(window, x, y);
}

static void syscall_scroll(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	uint16_t n = regs->ecx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_scroll(window, n);
}

static void syscall_width(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_width(window);
}

static void syscall_height(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_height(window);
}

static void syscall_clear_line(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	uint16_t n = regs->ecx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_clear_line(window, n);
}

static void syscall_set_visible(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	bool vis = regs->ecx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_set_visible(window, vis);
}

static void syscall_redraw(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_redraw(window);
}

static void syscall_get_x(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	
	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_x(window);
}

static void syscall_get_y(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) {
		regs->eax = 0;
		return;
	}

	regs->eax = window_get_y(window);
}

static void syscall_restore_cursor(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_restore_cursor(window);
}

static void syscall_reposition(registers_t* regs) {
	window_t* window = (window_t*)regs->ebx;
	uint16_t x = regs->ecx;
	uint16_t y = regs->edx;
	uint16_t width = regs->esi;
	uint16_t height = regs->edi;

	process_t* process = process_get_running();
	// TODO: Error (?)
	if(!process->foreground) return;

	window_reposition(window, x, y, width, height);
}

void window_register_syscalls() {
	syscall_register(SYSCALL_WINDOW_CREATE, syscall_create);
	syscall_register(SYSCALL_WINDOW_DELETE, syscall_delete);
	syscall_register(SYSCALL_WINDOW_WRITE, syscall_write);
	syscall_register(SYSCALL_WINDOW_CLEAR, syscall_clear);
	syscall_register(SYSCALL_WINDOW_SET_TEXT_COLOR, syscall_set_text_color);
	syscall_register(SYSCALL_WINDOW_SET_BACKGROUND_COLOR, syscall_set_background_color);
	syscall_register(SYSCALL_WINDOW_GET_TEXT_COLOR, syscall_get_text_color);
	syscall_register(SYSCALL_WINDOW_GET_BACKGROUND_COLOR, syscall_get_background_color);
	syscall_register(SYSCALL_WINDOW_GET_CURSOR_X, syscall_get_cursor_x);
	syscall_register(SYSCALL_WINDOW_GET_CURSOR_Y, syscall_get_cursor_y);
	syscall_register(SYSCALL_WINDOW_SET_CURSOR_POS, syscall_set_cursor_pos);
	syscall_register(SYSCALL_WINDOW_SCROLL, syscall_scroll);
	syscall_register(SYSCALL_WINDOW_WIDTH, syscall_width);
	syscall_register(SYSCALL_WINDOW_HEIGHT, syscall_height);
	syscall_register(SYSCALL_WINDOW_CLEAR_LINE, syscall_clear_line);
	syscall_register(SYSCALL_WINDOW_SET_VISIBLE, syscall_set_visible);
	syscall_register(SYSCALL_WINDOW_REDRAW, syscall_redraw);
	syscall_register(SYSCALL_WINDOW_GET_X, syscall_get_x);
	syscall_register(SYSCALL_WINDOW_GET_Y, syscall_get_y);
	syscall_register(SYSCALL_WINDOW_RESTORE_CURSOR, syscall_restore_cursor);
	syscall_register(SYSCALL_WINDOW_REPOSITION, syscall_reposition);
}

//////////////////////////////////////////////////////////////
//// implementation
//////////////////////////////////////////////////////////////

static void update_native_cursor() {
	window_t* window = term_native();

	// if this is a native terminal set the native cursor position
	int offset = window->cursor_x + window->cursor_y * window->width;

	// 0x3d4 - vga command register
	// 0x3d5 - vga data register
	outb(0x3d4, 14);
	outb(0x3d5, (uint8_t)(offset >> 8));
	outb(0x3d4, 15);
	outb(0x3d5, (uint8_t)(offset & 0xff));
}

window_t* window_create(window_t* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible) {
	// if the parent is invalid set the parent to the native window
	if(parent == NULL || !IS_WINDOW(parent)) {
		parent = term_native();
	}

	window_t* window = (window_t*)malloc(sizeof(window_t));

	window->magic = WINDOW_MAGIC;
	window->parent = parent;
	window->x = x;
	window->y = y;
	window->width = width;
	window->height = height;
	window->fg_col = 0x0;
	window->bg_col = 0xf;
	window->visible = visible;
	window->screen_buffer = (cell_t*)malloc(sizeof(cell_t) * width * height);

	return window;
}

void window_delete(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	// can not delete native window
	if(term_native() == window) {
		return;
	}

	memset(window, NULL, sizeof(window_t));

	free(window->screen_buffer);
	free(window);
}

void window_delete_for_process(uint32_t uid) {
	UNUSED(uid);

	// TODO: probably should store somewhere all the window pointers so i can handle their memory on the kernel side...
}

void window_write(window_t* window, const char* text) {
	// make sure this is a window
	if(!IS_WINDOW(window)) return;

	while (*text != 0) {
		char c = *text;
		switch (c) {
			case '\n': {
				window->cursor_y++;
				window->cursor_x = 0;
			} break;
			case '\t': {
				// tab is two spaces
				cell_t* cell = &window->screen_buffer[window->cursor_x + window->cursor_y * window->width];
				cell->chr = ' ';
				cell->bg = window->bg_col;
				cell->fg = window->fg_col;

				cell = &window->screen_buffer[1 + window->cursor_x + window->cursor_y * window->width];
				cell->chr = ' ';
				cell->bg = window->bg_col;
				cell->fg = window->fg_col;

				window->cursor_x += 2;
			} break;
			default: {
				cell_t* cell = &window->screen_buffer[window->cursor_x + window->cursor_y * window->width];
				cell->chr = c;
				cell->bg = window->bg_col;
				cell->fg = window->fg_col;

				window->cursor_x++;
			} break;
		}
		text++;

		// go to next line if needed
		if (window->cursor_x >= window->width) {
			window->cursor_y++;
			window->cursor_x = 0;
		}

		// scroll if needed
		if (window->cursor_y >= window->height) {
			window->cursor_y--;
			window->cursor_x = 0;
			window_scroll(window, 1);
		}
	}

	// simple hack to update the cursor positions in the parents 
	window_set_cursor_pos(window, window->cursor_x, window->cursor_y);

	if (window->visible) {
		window_redraw(window);
	}
}

void window_clear(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	for (int i = 0; i < window->width * window->height; i++) {
		cell_t* cell = &window->screen_buffer[i];
		cell->chr = 0;
		cell->bg = window->bg_col;
		cell->fg = window->fg_col;
	}

	if (window->visible) {
		window_redraw(window);
	}
}

void window_set_text_color(window_t* window, uint8_t color) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	window->fg_col = color;
}

void window_set_background_color(window_t* window, uint8_t color) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	window->bg_col = color;
}

uint8_t window_get_text_color(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	return window->fg_col;
}

uint8_t window_get_background_color(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	return window->bg_col;
}

void window_set_cursor_pos(window_t* window, uint16_t x, uint16_t y) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	window->cursor_x = x;
	window->cursor_y = y;

	if(window->parent != NULL) {
		window_set_cursor_pos(window->parent, window->cursor_x + window->x, window->cursor_y + window->y);
	}else {
		update_native_cursor(window);
	}
}

uint16_t window_get_cursor_x(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	return window->cursor_x;
}

uint16_t window_get_cursor_y(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	return window->cursor_y;
}

uint16_t window_get_width(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	return window->width;
}

uint16_t window_get_height(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	return window->height;
}

void window_scroll(window_t* window, uint16_t n) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	int length_copy = ((window->height - n) * window->width);
	int length_remove = (n * window->width);

	for (int i = 0; i < length_copy; i++) {
		window->screen_buffer[i] = window->screen_buffer[i + (n * window->width)];
	}

	for (int i = 0; i < length_remove; i++) {
		cell_t* cell = &window->screen_buffer[i];
		cell->chr = 0;
		cell->bg = window->bg_col;
		cell->fg = window->fg_col;
	}

	if (window->visible) {
		window_redraw(window);
	}
}

void window_clear_line(window_t* window, uint16_t n) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	int offset = (n * window->width);
	for (int i = offset; i < window->width + offset; i++) {
		cell_t* cell = &window->screen_buffer[i];
		cell->chr = 0;
		cell->bg = window->bg_col;
		cell->fg = window->fg_col;
	}

	if (window->visible) {
		window_redraw(window);
	}
}

void window_set_visible(window_t* window, bool vis) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	// native is always visible
	if(term_native() == window) {
		return;
	}

	window->visible = vis;
	if (vis) {
		window_redraw(window);
	}
}

void window_redraw(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	// can not redraw native window
	if(term_native() == window) {
		return;
	}

	if (!window->visible) return;

	uint16_t width = MIN(window->width, window->parent->width - window->width + window->x);
	uint16_t height = MIN(window->height, window->parent->height - window->height + window->y);

	for (uint16_t y = 0; y < height; y++) {
		uint16_t screenY = window->y + y;
		for (uint16_t x = 0; x < width; x++) {
			uint16_t screenX = window->x + x;
			window->parent->screen_buffer[screenX + screenY * window->parent->width] = window->screen_buffer[x + y * window->width];
		}
	}
}

uint16_t window_get_x(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	// x is always 0 on native window
	if(term_native() == window) {
		return 0;
	}

	return window->cursor_x;
}

uint16_t window_get_y(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return 0;

	// y is always 0 on native window
	if(term_native() == window) {
		return 0;
	}

	return window->cursor_y;
}

void window_restore_cursor(window_t* window) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	// can not restore cursor of native window
	if(term_native() == window) {
		return;
	}

	window->parent->cursor_x = MIN(window->x + window->cursor_x, window->parent->width - 1);
	window->parent->cursor_y = MIN(window->y + window->cursor_y, window->parent->height - 1);
}

void window_reposition(window_t* window, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	// make sure this is a valid window
	if(!IS_WINDOW(window)) return;

	// can not reposition native window
	if(term_native() == window) {
		return;
	}

	if (window->width != width || window->height != height) {
		cell_t* newScreenBuffer = (cell_t*)malloc(sizeof(cell_t) * width * height);

		uint16_t copyWidth = MIN(width, window->width);
		uint16_t copyHeight = MIN(height, window->height);

		for (uint16_t cy = 0; cy < copyHeight; cy++) {
			for (uint16_t cx = 0; cx < copyWidth; cx++) {
				newScreenBuffer[cx + cy * width] = window->screen_buffer[cx + cy * window->width];
			}
		}

		free(window->screen_buffer);

		window->screen_buffer = newScreenBuffer;
		window->width = width;
		window->height = height;
	}

	window->x = x;
	window->y = y;

	if (window->visible) {
		window_redraw(window);
	}
}
