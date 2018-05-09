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
#include <api/coroutine.h>

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

void program(void* arg) {
	// actual program
	os_pull_event(EVENT_KEY);
	printf("pressed!");
}

void kmain(void) {
	kernel_init();

	// the main program coroutine
	coroutine_t coro = coroutine_create(program);
	event_t lastEvent;
	while (true) {
		// resume the coroutine with an event
		// in the first run it will ignore the event
		// whenever the programs yields out of it's coroutine (by default
		// it should only be when calling `poll_event_raw`) the kernel will halt until
		// an input is received
		int filter = coroutine_resume(&coro, &lastEvent);

		// did the program finish? if so exit
		if (coroutine_status(&coro) == COROUTINE_STATUS_DEAD) {
			break;
		}

		// call the kernel poll event (a blocking function)
		lastEvent = kernel_poll_event(filter);
	}
}