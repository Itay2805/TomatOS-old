#include "timer.h"

#include "drivers.h"
#include "ports.h"
#include "isr.h"

#define DRIVERS_TIMER_HARWARE_CLOCK_FREQ	1193180 
#define DRIVERS_TIMER_FREQ					1000

#include <api/os.h>

#include <string.h>
#include <memory.h>
#include <stddef.h>

typedef struct __timer_t {
	uint32_t id;
	uint32_t milis_left;
	struct __timer_t* next;
} __timer_t;

__timer_t* base;

// TODO: Maybe have the timer driver handle the id generation

static void __timer_callback(registers_t regs) {
	UNUSED(regs);

	if (base == nullptr) return;
	__timer_t* curr = base;
	__timer_t* before = nullptr;
	while (curr != nullptr) {
		curr->milis_left -= 1000 / DRIVERS_TIMER_FREQ;
		if (curr->milis_left <= 0) {
			if (before != nullptr) {
				before->next = curr->next;
			}
			else {
				base = curr->next;
			}
			event_t event;
			timer_t* timerEvent = (timer_t*)&event;
			timerEvent->type = EVENT_TIMER;
			timerEvent->id = curr->id;
			os_queue_event(event);
			free(curr);
		}
		else {
			before = curr;
		}
		curr = curr->next;
	}
}

void timer_create(uint32_t id, uint32_t milis) {
	__timer_t* timer = (__timer_t*)malloc(sizeof(__timer_t));
	timer->id = id;
	timer->milis_left = milis;
	timer->next = base;
	base = timer;
}

void timer_cancel(uint32_t id) {
	if (base == nullptr) return;
	__timer_t* curr = base;
	__timer_t* before = nullptr;
	while (curr != nullptr) {
		if (curr->id == id) {
			if (before != nullptr) {
				before->next = curr->next;
			}
			else {
				base = curr->next;
			}
			free(curr);
		}
		else {
			before = curr;
		}
		curr = curr->next;
	}
}

void kernel_timer_init() {
	register_interrupt_handler(DRIVERS_TIMER_IRQ, __timer_callback);

	//uint32_t divisor = DRIVERS_TIMER_HARWARE_CLOCK_FREQ / DRIVERS_TIMER_FREQ;
	uint32_t divisor = DRIVERS_TIMER_FREQ;
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);

	base = nullptr;
}