#include "timer.h"

#include <boot/interrupts/pic.h>
#include <boot/interrupts/isr.h>
#include <core/process/process.h>

#include <stdbool.h>
#include <core/sbuf.h>


#define PIT_COMMAND 0x43
#define PIT_DATA1   0x40
#define PIT_DATA2   0x41
#define PIT_DATA3   0x42
#define PIT_SET		0x36

static uint32_t ids = 0;

typedef struct timer {
	uint32_t id;
	uint32_t uid;
	uint32_t millis_left;
	bool finished;
} timer_t;

static timer_t* timers = 0;

static void handle_timer_irq(registers_t* regs) {
	for (timer_t* timer = timers; timer < buf_end(timers); timer++) {
		timer->millis_left -= 10;
		if (timer->millis_left <= 0) {
			// timer finished, add the event
			event_t event;
			event.type = EVENT_TIMER;
			event.data[0] = timer->id;
			buf_push(process_get(timer->uid)->events, event);
		}
	}

	scheduler_update(regs, true);
}

void timer_init(void) {
	// set interrupt handler
	register_irq_handler(IRQ_TIMER, handle_timer_irq);

	// initialize pit
	uint32_t divisor = 1193180 / (1000 / 10);
	outb(PIT_COMMAND, PIT_SET);
	outb(PIT_DATA1, divisor & 0xFF);
	outb(PIT_DATA1, (divisor >> 8) & 0xFF);
}

uint32_t timer_start(uint32_t uid, uint32_t millis) {
	timer_t newTimer;
	newTimer.uid = uid;
	newTimer.id = ids++;
	newTimer.millis_left = millis;
	newTimer.finished = false;

	for (timer_t* timer = timers; timer < buf_end(timers); timer++) {
		if (timer->finished == true) {
			*timer = newTimer;
			return timer->id;
		}
	}

	buf_push(timers, newTimer);
	return newTimer.id;
}

void timer_cancel(uint32_t id) {
	for (timer_t* timer = timers; timer < buf_end(timers); timer++) {
		if (timer->id == id) {
			timer->finished = true;
		}
	}
}
