// #include <term.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#include <api/term.h>
#include <api/color.h>
#include <api/window.h>
#include <api/os.h>
#include <api/keys.h>

#include "../drivers/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "idt.h"

void kernel_init() {

	// init native terminal
	kernel_init_native_term();
	term_set_background_color(COLOR_BLACK);
	term_set_text_color(COLOR_WHITE);
	term_clear();

	// install isrs
	kernel_isr_install();

	// enable interrupts
#ifndef VISUAL_STUDIO
	asm volatile("sti");
#endif

	// init keyboard driver
	kernel_timer_init();
	kernel_keyboard_init();

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

	timer_t timer = os_start_timer(10);

	term_write("watiting 10 seconds..\n");

	event_t event;
	while (true) {
		event = os_pull_event_raw(EVENT_TIMER);
		switch (event.type) {
			case EVENT_TIMER: {
				timer_t* timerEvent = (timer_t*)&event;
				if (timerEvent->id == timer.id) {
					term_write("10 seconds have passed\n");
				}
			} break;
		}
	}
}