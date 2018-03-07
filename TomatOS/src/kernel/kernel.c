// #include <term.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include <api/term.h>
#include <api/color.h>
#include <api/window.h>
#include <api/os.h>

#include "isr.h"
#include "idt.h"

void kernel_init() {
	kernel_isr_install();

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

void kmain(void) {
	kernel_init();

	term_set_text_color(COLOR_RED);
	term_set_background_color(COLOR_BLACK);
	term_clear();
	term_set_cursor_pos(0, 0);

#ifndef VISUAL_STUDIO
	__asm__ __volatile__("int $3");
#endif
#ifndef VISUAL_STUDIO
	__asm__ __volatile__("int $1");
#endif

	event_t event;
	while (true) {
		event = os_pull_event_raw(EVENT_ALL);
		switch (event.type) {
			case EVENT_EXCEPTION: {
				exception_t* exception = (exception_t*)&event;
				term_write(exception->msg);
				term_write("\n");
				
			} break;
		}
	}

	drawMenu();
}