// #include <term.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include <api/term.h>
#include <api/color.h>
#include <api/window.h>

void kmain(void) {
	// init native terminal
    kernel_init_native_term();
	term_set_background_color(COLOR_BLACK);
	term_set_text_color(COLOR_WHITE);
	term_clear();

	// init memory management
    kernel_memory_init();

	// init window
	kernel_init_window();

	term_t* original_term = term_current();
	window_t window = window_create(original_term, 4, 2, 20, 10, true);
	term_redirect(window_as_term(&window));

	term_set_background_color(COLOR_RED);
	term_clear();

	// need to make so this is called automatically
	window_redraw(&window);

	term_redirect(original_term);
	window_destroy(&window);
}