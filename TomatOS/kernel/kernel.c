// #include <term.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include <api/term.h>
#include <api/color.h>
#include <api/window.h>

void kernel_init() {
	// init native terminal
	kernel_init_native_term();
	term_set_background_color(COLOR_BLACK);
	term_set_text_color(COLOR_WHITE);
	term_clear();

	// init memory management
	kernel_memory_init();

	// init window
	kernel_init_window();
}

void clear() {
	term_set_cursor_pos(0, 0);
	term_set_background_color(COLOR_BLACK);
	term_set_text_color(COLOR_WHITE);
	term_clear();
}

#define STRLEN(str) (sizeof(str) - 1)

term_t* oldTerm;
window_t olist;
window_t cmd;

void redrawList() {
	window_set_background_color(&olist, COLOR_GRAY);
	window_clear(&olist);
	window_set_cursor_pos(&olist, 0, 0);
	window_set_text_color(&olist, COLOR_WHITE);
	term_redirect(&olist);

	// draw a list of options
	
	term_redirect(oldTerm);
}

void redrawCommand() {
	term_redirect(&cmd);

	term_set_cursor_pos(0, 0);
	term_set_background_color(COLOR_GRAY);
	term_set_text_color(COLOR_LIGHT_GREEN);
	term_clear();
	
	// enter command
	term_write("NONE");

	term_redirect(oldTerm);
}

void drawMenu() {
	clear();

	oldTerm = term_current();
	uint16_t w = 30, h = 11;
	olist = window_create(term_current(), 40 - w / 2, 9 - h / 2, w, h, true);
	char str[] = "-TomatOS-";
	char str2[] = "Please select an option.";
	char str3[] = "Press E to write additional commands.";
	cmd = window_create(term_current(), 40 - STRLEN(str3) / 2, 18, STRLEN(str3), 1, true);
	window_set_background_color(&cmd, COLOR_GRAY);
	window_clear(&cmd);
	term_set_cursor_pos(40 - STRLEN(str) / 2, 0);
	term_write(str);
	term_set_cursor_pos(40 - STRLEN(str2) / 2, 1);
	term_write(str2);
	term_set_cursor_pos(40 - STRLEN(str3) / 2, 17);
	term_write(str3);

	redrawList();
	redrawCommand();

	/// pull events

	window_destroy(&olist);
	window_destroy(&cmd);
}

void kmain(void) {
	kernel_init();
	
	drawMenu();
}