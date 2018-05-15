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

static void interrupt_keyboard_handle(registers_t* regs) {
	uint8_t scancode = inb(0x60);
	event_t event;

	char buf[256];
	itoa(scancode, buf, 10);
	term_kwrite(buf);
	term_kwrite("\n");

	if (scancode <= 0x7f) {		
		uint8_t vkcode = vkcodes[scancode];

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
			if (chars[scancode] != 0) {
				event.type = TOMATO_EVENT_CHAR;
				event.data[0] = chars[scancode];
				event.data[1] = 0;
				os_kqueue_event(event);
			}
		}

		keystates[vkcode] = true;
	}
	else {

		uint8_t vkcode = vkcodes[scancode + SCANCODE_LIMIT];

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
		vkcodes[2 + i] = '0' + i;
	}
	
	// A - Z
	chars[10] = 'A';
	chars[11] = 'B';
	chars[30] = 'C';
	chars[48] = 'D';
	chars[46] = 'E';
	chars[32] = 'F';
	chars[18] = 'G';
	chars[33] = 'H';
	chars[34] = 'I';
	chars[35] = 'J';
	chars[23] = 'K';
	chars[36] = 'L';
	chars[37] = 'M';
	chars[38] = 'N';
	chars[50] = 'O';
	chars[49] = 'P';
	chars[24] = 'Q';
	chars[25] = 'R';
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

	// TODO: NUMPAD keys
	// TODO: backspace - 14
}


