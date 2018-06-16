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

#define DEBUG_KEYBOARD 1

static void interrupt_keyboard_handle(registers_t* regs) {
	uint8_t scancode = inb(0x60);

	if (scancode <= 0x7f) {
		uint8_t vkcode = vkcodes[scancode];
		if (vkcode == 0) return;

		// key press
		event_t keyevent;
		keyevent.type = TOMATO_EVENT_KEY;
		keyevent.data[0] = vkcode;
		keyevent.data[1] = keystates[vkcode];
		keyevent.data[2] = 0;
		keyevent.data[3] = 0;
		os_kqueue_event(keyevent);

		// char event
		if (!keystates[vkcode] && vkcode != TOMATO_KEYS_SHIFT_LEFT && vkcode != TOMATO_KEYS_SHIFT_RIGHT) {
			if (keystates[TOMATO_KEYS_SHIFT_LEFT] || keystates[TOMATO_KEYS_SHIFT_RIGHT]) {
				scancode += SCANCODE_LIMIT;
			}

			if (chars[scancode] != 0) {
				event_t charevent;
				charevent.type = TOMATO_EVENT_CHAR;
				charevent.data[0] = chars[scancode];
				charevent.data[1] = 0;
				charevent.data[2] = 0;
				charevent.data[3] = 0;
				os_kqueue_event(charevent);
			}
		}

		keystates[vkcode] = true;
	}
	else {
		// key release
		uint8_t vkcode = vkcodes[scancode - 0x80];
		if (vkcode == 0) return;

		event_t event;
		event.type = TOMATO_EVENT_KEY_UP;
		event.data[0] = vkcode;
		event.data[1] = 0;
		event.data[2] = 0;
		event.data[3] = 0;

		os_kqueue_event(event);

		keystates[vkcode] = false;
	}
}

void driver_keyboard_init(void) {
	register_interrupt_handler(IRQ_KEYBOARD, interrupt_keyboard_handle);
	
	// clear the memory
	memset(chars, 0, sizeof(chars));

	// 0 - 9
	chars[2] = '1';
	chars[3] = '2';
	chars[4] = '3';
	chars[5] = '4';
	chars[6] = '5';
	chars[7] = '6';
	chars[8] = '7';
	chars[9] = '8';
	chars[10] = '9';
	chars[11] = '0';
	
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

	// these keys have the same virtual key code as their value
	memcpy(vkcodes, chars, 256);
	
	/* collides with arrow keys
	// numpad
	chars[82] = '0';
	vkcodes[82] = TOMATO_KEYS_NUMPAD0;

	chars[79] = '1';
	vkcodes[79] = TOMATO_KEYS_NUMPAD1;
	
	chars[80] = '2';
	vkcodes[80] = TOMATO_KEYS_NUMPAD2;
	
	chars[81] = '3';
	vkcodes[81] = TOMATO_KEYS_NUMPAD3;
	
	chars[75] = '4';
	vkcodes[75] = TOMATO_KEYS_NUMPAD4;
	
	chars[76] = '5';
	vkcodes[76] = TOMATO_KEYS_NUMPAD5;
	
	chars[77] = '6';
	vkcodes[77] = TOMATO_KEYS_NUMPAD6;
	
	chars[71] = '7';
	vkcodes[71] = TOMATO_KEYS_NUMPAD7;
	
	chars[72] = '8';
	vkcodes[72] = TOMATO_KEYS_NUMPAD8;
	
	chars[73] = '9';
	vkcodes[73] = TOMATO_KEYS_NUMPAD9;
	*/

	// special characters are offseted
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

	// shift keys
	vkcodes[42] = TOMATO_KEYS_SHIFT_LEFT;
	vkcodes[54] = TOMATO_KEYS_SHIFT_RIGHT;
	
	// ctrl keys
	vkcodes[29] = TOMATO_KEYS_LEFT_CTRL;
	//vkcodes[] = TOMATO_KEYS_RIGHT_CTRL;

	// enter
	vkcodes[28] = TOMATO_KEYS_ENTER;

	// arrow keys
	vkcodes[72] = TOMATO_KEYS_UP;
	vkcodes[75] = TOMATO_KEYS_LEFT;
	vkcodes[80] = TOMATO_KEYS_DOWN;
	vkcodes[77] = TOMATO_KEYS_RIGHT;

	// TODO: backspace - 14
}


