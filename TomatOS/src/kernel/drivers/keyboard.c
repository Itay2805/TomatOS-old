#include "keyboard.h"

#include "../interrupt.h"
#include "../port.h"

#include "../syscalls/os.h"
#include "../syscalls/term.h"

#include <stdbool.h>
#include <string.h>

#include <tomato.h>

#define SCANCODE_LIMIT (0x7f + 1)

static bool keystates[256];
static char vkcodes[256];
static char chars[256];

#define DEBUG_KEYBOARD 0

static void interrupt_keyboard_handle(registers_t* regs) {
	uint8_t scancode = inb(0x60);
	event_t event;
	
#if DEBUG_KEYBOARD
	char buf[256];
	itoa(scancode, buf, 10);
	term_kwrite("scancode: ");
	term_kwrite(buf);
	term_kwrite("\n");
#endif

	if (scancode <= 0x7f) {
		uint8_t vkcode = vkcodes[scancode];
		if (vkcode == 0) return;
		
#if DEBUG_KEYBOARD
		char buf[2];
		buf[0] = vkcode;
		buf[1] = 0;
		term_kwrite("vkcode: ");
		term_kwrite(buf);
		term_kwrite("\n");
#endif

		// key press
		event.type = TOMATO_EVENT_KEY;
		event.data[0] = vkcode;
		event.data[1] = keystates[vkcode];
		event.data[2] = 0;
		event.data[3] = 0;
		os_kqueue_event(event);

		// char event
		if (!keystates[vkcode]) {
			if (keystates[TOMATO_KEYS_SHIFT_LEFT] || keystates[TOMATO_KEYS_SHIFT_RIGHT]) {
				scancode += SCANCODE_LIMIT;
			}

#if DEBUG_KEYBOARD
			term_kwrite("first press\n");
#endif

			if (chars[scancode] != 0) {
#if DEBUG_KEYBOARD
				char buf[2];
				buf[0] = chars[scancode];
				buf[1] = 0;
				term_kwrite("char: ");
				term_kwrite(buf);
				term_kwrite("\n");
#endif

				event.type = TOMATO_EVENT_CHAR;
				event.data[0] = chars[scancode];
				event.data[1] = 0;
				os_kqueue_event(event);
			}
		}

		keystates[vkcode] = true;
	}
	else {
		uint8_t vkcode = vkcodes[scancode - 0x7f];
		if (vkcode == 0) return;

		// key release
		scancode -= 0x80;
		event.type = TOMATO_EVENT_KEY_UP;
		event.data[0] = vkcode;
		event.data[1] = 0;
		event.data[2] = 0;
		event.data[3] = 0;

		keystates[vkcode] = false;
	}
}

void driver_keyboard_init(void) {
	register_interrupt_handler(IRQ_KEYBOARD, interrupt_keyboard_handle);
	
	// 0 - 9
	for (int i = 0; i < 10; i++) {
		chars[2 + i] = '0' + i;
	}
	
	// A - Z
	chars[30] = 'A';
	chars[48] = 'B';
	chars[46] = 'C';
	chars[32] = 'D';
	chars[18] = 'E';
	chars[33] = 'F';
	chars[34] = 'G';
	chars[35] = 'H';
	chars[23] = 'I';
	chars[36] = 'J';
	chars[37] = 'K';
	chars[38] = 'L';
	chars[50] = 'M';
	chars[49] = 'N';
	chars[24] = 'O';
	chars[25] = 'P';
	chars[16] = 'Q';
	chars[19] = 'R';
	chars[31] = 'S';
	chars[20] = 'T';
	chars[22] = 'U';
	chars[47] = 'V';
	chars[17] = 'W';
	chars[45] = 'X';
	chars[21] = 'Y';
	chars[44] = 'Z';

	chars[57] = ' ';

	// numpad
	chars[82] = '0';
	chars[79] = '1';
	chars[80] = '2';
	chars[81] = '3';
	chars[75] = '4';
	chars[76] = '5';
	chars[77] = '6';
	chars[71] = '7';
	chars[72] = '8';
	chars[73] = '9';

	memcpy(vkcodes, chars, 256);

	chars[SCANCODE_LIMIT + 2] = '!';
	chars[SCANCODE_LIMIT + 3] = '@';
	chars[SCANCODE_LIMIT + 4] = '#';
	chars[SCANCODE_LIMIT + 5] = '$';
	chars[SCANCODE_LIMIT + 6] = '%';
	chars[SCANCODE_LIMIT + 7] = '^';
	chars[SCANCODE_LIMIT + 8] = '&';
	chars[SCANCODE_LIMIT + 9] = '*';
	chars[SCANCODE_LIMIT + 10] = '(';
	chars[SCANCODE_LIMIT + 11] = ')';

	vkcodes[42] = TOMATO_KEYS_SHIFT_LEFT;
	vkcodes[54] = TOMATO_KEYS_SHIFT_RIGHT;
	
	// TODO: backspace - 14
}


