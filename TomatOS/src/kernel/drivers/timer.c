#include "timer.h"

#include "../syscalls/os.h"
#include "../syscalls/term.h"

#include "../port.h"
#include "../interrupt.h"
#include "../heap.h"

#include <stddef.h>

#define PIT_COMMAND 0x43
#define PIT_DATA1   0x40
#define PIT_DATA2   0x41
#define PIT_DATA3   0x42
#define PIT_SET		0x36

#define DRIVER_TIMER_INTERVAL 10

typedef struct timer_node_t {
	uint32_t id;
	uint8_t enabled;
	int32_t milis_left;
	struct timer_node_t* next;
} timer_node_t;

static timer_node_t* base;
static uint32_t lastid = 0;

uint32_t driver_start_timer(uint32_t milis) {
	timer_node_t* timer = (timer_node_t*)heap_allocate(sizeof(timer_node_t));
	timer->id = lastid++;
	timer->milis_left = milis;
	timer->next = base;
	base = timer;
	return timer->id;
}

void driver_cancel_timer(uint32_t id) {
	if (base == NULL) return;
	timer_node_t* curr = base;
	timer_node_t* before = NULL;
	while (curr != NULL) {
		timer_node_t* next = curr->next;

		if (curr->id == id) {
			if (before != NULL) {
				before->next = curr->next;
			}
			else {
				base = curr->next;
			}
			heap_free(curr);
		}
		else {
			before = curr;
		}

		curr = next;
	}
}

static void interrupt_timer_handle(registers_t* regs) {
	UNUSED(regs);

	if (base == NULL) return;
	timer_node_t* curr = base;
	timer_node_t* before = NULL;
	while (curr != NULL) {
		timer_node_t* next = curr->next;

		curr->milis_left -= DRIVER_TIMER_INTERVAL;
		if (curr->milis_left <= 0) {
			if (before != NULL) {
				before->next = curr->next;
			}
			else {
				base = curr->next;
			}
			event_t event;
			event.type = TOMATO_EVENT_TIMER;
			event.data[0] = curr->id;
			os_kqueue_event(event);
			heap_free(curr);
		}
		else {
			before = curr;
		}

		curr = next;
	}
}

void driver_timer_init(void) {
	// set interrupt handler
	register_interrupt_handler(IRQ_TIMER, interrupt_timer_handle);
	base = NULL;

	// initialize pit
	uint32_t divisor = 1193180 / (1000 / DRIVER_TIMER_INTERVAL);
	outb(PIT_COMMAND, PIT_SET);
	outb(PIT_DATA1, divisor & 0xFF);
	outb(PIT_DATA1, (divisor >> 8) & 0xFF);
}

#undef PIT_COMMAND
#undef PIT_DATA1
#undef PIT_DATA2
#undef PIT_DATA3