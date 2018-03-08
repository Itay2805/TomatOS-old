#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "drivers.h"

#include <api/os.h>
#include <api/term.h>

#include <stdbool.h>

bool __key_states[0x7f];

static void __keyboard_callback(registers_t regs) {
	uint8_t scancode = port_byte_in(0x60);
	event_t event;
	if (scancode <= 0x7f) {
		// key press
		key_event_t* e = (key_event_t*)&event;
		e->type = EVENT_KEY;
		e->scancode = scancode;
		e->held = __key_states[scancode];
		__key_states[scancode] = true;

		// if the key is a letter we should also spawn a char event
	}
	else {
		// key up
		scancode -= 0x80;
		key_up_event_t* e = (key_up_event_t*)&event;
		e->type = EVENT_KEY_UP;
		e->scancode = scancode;
		__key_states[scancode] = false;
	}
	os_queue_event(event);
}

void kernel_keyboard_init(void) {
	register_interrupt_handler(DRIVERS_KEYBOARD_IRQ, __keyboard_callback);
}