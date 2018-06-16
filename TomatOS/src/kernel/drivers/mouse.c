#include "mouse.h"

#include "../interrupt.h"
#include "../port.h"

#include "../syscalls/os.h"
#include "../syscalls/term.h"

#include <tomato.h>

#define DATA_PORT 0x60
#define COMMAND_PORT 0x64

typedef struct ps2_info_t {
	uint8_t left_btn : 1;
	uint8_t right_btn : 1;
	uint8_t middle_btn : 1;
	uint8_t always_one : 1;
	uint8_t x_sign : 1;
	uint8_t y_sign : 1;
	uint8_t x_overflow : 1;
	uint8_t y_overflow : 1;
} ps2_info_t;

static uint8_t buffer[3];
static uint8_t offset;
static ps2_info_t info;
static float mouse_x, mouse_y;

static void check_mouse_btn(uint8_t id, uint8_t now, uint8_t old) {
	if (now != old) {
		if (now) {
			event_t e;
			e.type = TOMATO_EVENT_MOUSE_CLICK;
			e.data[0] = id;
			e.data[1] = (int8_t)mouse_x;
			e.data[2] = (int8_t)mouse_y;
			e.data[3] = 0;
			os_kqueue_event(e);
		}
		else {
			event_t e;
			e.type = TOMATO_EVENT_MOUSE_UP;
			e.data[0] = id;
			e.data[1] = (int8_t)mouse_x;
			e.data[2] = (int8_t)mouse_y;
			e.data[3] = 0;
			os_kqueue_event(e);
		}
	}
	else if(old) {
		event_t e;
		e.type = TOMATO_EVENT_MOUSE_DRAG;
		e.data[0] = id;
		e.data[1] = (int8_t)mouse_x;
		e.data[2] = (int8_t)mouse_y;
		e.data[3] = 0;
	}
}

#define VIDEO_ADDRESS 0xb8000
static uint8_t* vram;
static uint8_t before;
static uint8_t before_color;
static uint8_t* before_addr;

void driver_mouse_restore_screen() {
	if (before_addr != 0) {
		*before_addr = before;
		*(before_addr + 1) = before_color;
	}
}

void driver_mouse_draw() {
	return;

	driver_mouse_restore_screen();
	before_addr = &vram[((int8_t)mouse_x + (int8_t)mouse_y * 80) * 2];
	before = *before_addr;
	before_color = *(before_addr + 1);
	*before_addr = 127;

	uint8_t bg = (before_color >> 4) & 0xF;
	if (bg >= 0 && bg <= 6 || bg == 8) {
		*(before_addr + 1) = bg << 4 | 0xf;
	}
	else {
		*(before_addr + 1) = bg << 4 | 0x0;
	}

}

static void interrupt_mouse_handle(registers_t* regs) {
	buffer[offset] = inb(DATA_PORT);

	offset = (offset + 1) % 3;

	if (offset == 0) {
		ps2_info_t i = *(ps2_info_t*)&buffer[0];
		if (i.x_overflow || i.y_overflow) return;
		
		float dx = (buffer[1] > 0 ? 0.5f : 0);
		float dy = -(buffer[2] > 0 ? 0.5f : 0);

		if (i.x_sign) dx = -dx;
		if (i.y_sign) dy = -dy;

		mouse_x += dx;
		mouse_y += dy;
		
		if (mouse_x < 0) mouse_x = 0;
		if (mouse_x >= 80) mouse_x = 79;

		if (mouse_y < 0) mouse_y = 0;
		if (mouse_y >= 25) mouse_y = 24;

		driver_mouse_draw();

		check_mouse_btn(TOMATO_MOUSE_LEFT, i.left_btn, info.left_btn);
		check_mouse_btn(TOMATO_MOUSE_RIGHT, i.right_btn, info.right_btn);
		check_mouse_btn(TOMATO_MOUSE_MIDDLE, i.middle_btn, info.middle_btn);
		info = i;
	}
}

static void wait_data() {
	int timeout = 100000;
	while (timeout--) {
		if ((inb(COMMAND_PORT) & 1) == 1) return;
	}
}

static void wait_signal() {
	int timeout = 100000;
	while (timeout--) {
		if ((inb(COMMAND_PORT) & 2) == 0) return;
	}
}

static void write(uint8_t data) {
	wait_signal();
	outb(COMMAND_PORT, 0xD4);
	wait_signal();
	outb(DATA_PORT, data);
}

static uint8_t read() {
	wait_data();
	return inb(DATA_PORT);
}

void driver_mouse_init(void) {
	offset = 0;
	
	vram = (uint8_t*)VIDEO_ADDRESS;
	before_addr = 0;

	wait_signal();
	outb(COMMAND_PORT, 0xA8);

	wait_signal();
	outb(COMMAND_PORT, 0x20);
	wait_data();
	uint8_t status = (inb(0x60) | 2);
	wait_signal();
	outb(COMMAND_PORT, 0x60);
	wait_signal();
	outb(DATA_PORT, status);

	write(0xF6);
	read();

	write(0xF4);
	read();

	register_interrupt_handler(IRQ_MOUSE, interrupt_mouse_handle);
}

#undef DATA_PORT
#undef COMMAND_PORT

