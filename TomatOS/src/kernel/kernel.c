// #include <term.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include <stdio.h>
#include <setjmp.h>

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

	// init drivers
	kernel_timer_init();
	kernel_keyboard_init();

	// init memory management
	kernel_memory_init();

	// init window
	kernel_init_window();
}

void kmain(void) {
	kernel_init();

	timer_t timer = os_start_timer(1000);
	bool waiting = true;
	while (waiting) {
		event_t event = os_pull_event(EVENT_ALL);
		switch (event.type) {
		case EVENT_TIMER: {
				timer_t * e = EVENT_CAST(timer_t);
				if (e->id == timer.id) {
					term_write("Timeout!\n");
					waiting = true;
				}
			}
			break;
		case EVENT_KEY: {
			key_event_t* e = EVENT_CAST(key_event_t);
				term_write("Pressed!\n");
				waiting = false;
			}
			break;
		}
	}
}