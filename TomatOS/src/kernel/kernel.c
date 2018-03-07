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
#include "idt.h"

void kernel_init() {
	// init native terminal
	kernel_init_native_term();
	term_set_background_color(COLOR_BLACK);
	term_set_text_color(COLOR_WHITE);
	term_clear();


	// install isrs
	kernel_isr_install();

	// init keyboard driver
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

	event_t event;
	while (true) {
		event = os_pull_event_raw(EVENT_KEY);
		switch (event.type) {
			case EVENT_KEY: {
				key_event_t* key = (key_event_t*)&event;
				switch (key->scancode) {
					case KEYS_A: term_write("A"); break;
					case KEYS_B: term_write("B"); break;
					case KEYS_C: term_write("C"); break;
					case KEYS_D: term_write("D"); break;
					case KEYS_E: term_write("E"); break;
					case KEYS_F: term_write("F"); break;
					case KEYS_G: term_write("G"); break;
					case KEYS_H: term_write("H"); break;
					case KEYS_I: term_write("I"); break;
					case KEYS_J: term_write("J"); break;
					case KEYS_K: term_write("K"); break;
					case KEYS_L: term_write("L"); break;
					case KEYS_M: term_write("M"); break;
					case KEYS_N: term_write("N"); break;
					case KEYS_O: term_write("O"); break;
					case KEYS_P: term_write("P"); break;
					case KEYS_Q: term_write("Q"); break;
					case KEYS_R: term_write("R"); break;
					case KEYS_S: term_write("S"); break;
					case KEYS_T: term_write("T"); break;
					case KEYS_U: term_write("U"); break;
					case KEYS_V: term_write("V"); break;
					case KEYS_W: term_write("W"); break;
					case KEYS_X: term_write("X"); break;
					case KEYS_Y: term_write("Y"); break;
					case KEYS_Z: term_write("Z"); break;
					case KEYS_SPACE: term_write(" "); break;
					case KEYS_ENTER: term_write("\n"); break;
				}
			} break;
		}
	}
}