#include "include/api/os.h"

#include <memory.h>
#include <string.h>

#include <api/term.h>
#include <api/color.h>
#include <api/coroutine.h>

#include "../drivers/timer.h"

#define API_OS_QUEUE_INC 5

static event_t* event_queue;
static int queue_cap;
static volatile int queue_front, queue_rear, queue_item_count;
static const char* label;
static uint32_t timer_id;


void kernel_os_init(void) {
	queue_item_count = 0;
	queue_front = 0;
	queue_rear = -1;
	queue_cap = API_OS_QUEUE_INC;
	event_queue = malloc(sizeof(event_t) * API_OS_QUEUE_INC);

	timer_id = 0;

	// TODO: somehow load label
	label = "<unlabeled>";
}

const char* os_version(void) {
	return "TomatOS";
}

const char* os_get_computer_label(void) {
	return label;
}

void os_set_computer_label(char* name) {
	label = name;
	// save label
}

// this is meant for the kernel only, it is a **blocking** poll event
// basically the normal one will yield, the idea is that techncially
// the program can catch that event yield, and do whatever it wants
// until it will eventually yield as well to get some actual input
event_t kernel_poll_event(uint32_t filter) {
	event_t event;
	do {
		while (queue_item_count == 0) {
			__asm__("");
		}
		event = event_queue[queue_front++];
		if (queue_front == queue_cap) {
			queue_cap = 0;
		}
		queue_item_count--;
	} while (filter != EVENT_ALL && event.type != filter);
	return event;
}

event_t os_pull_event_raw(uint32_t filter) {
	event_t event;
	do {
		while (queue_item_count == 0) {
			// if the queue is empty we yield
			return *(event_t*)coroutine_yield(filter);
		}
		event = event_queue[queue_front++];
		if (queue_front == queue_cap) {
			queue_cap = 0;
		}
		queue_item_count--;
	} while(filter != EVENT_ALL && event.type != filter);
	return event;
}

event_t os_pull_event(uint32_t filter) {
	event_t event = os_pull_event_raw(filter);

	// this is temp, will think for something better
	if (event.type == EVENT_TERMINATE) {
		// this means the program should be terminated...
		// need to think how to handle this :think:
		term_set_background_color(COLOR_BLACK);
		term_set_cursor_pos(0, 0);
		term_clear();
		term_set_text_color(COLOR_RED);
		term_write("Terminated");
	}
	else if (event.type == EVENT_EXCEPTION) {
		// also, the program should be terminated
		term_set_background_color(COLOR_RED);
		term_set_cursor_pos(0, 0);
		term_clear();
		term_set_text_color(COLOR_WHITE);
		exception_t* exception = (exception_t*)&event;
		term_write((char*)exception->msg);
	}

	return event;
}

void os_queue_event(event_t event) {
	if (queue_cap == queue_item_count) {
		// resize if needed
		event_queue = realloc(event_queue, queue_cap + API_OS_QUEUE_INC);
	}
	if (queue_rear == queue_cap - 1) {
		queue_rear = -1;
	}
	event_queue[++queue_rear] = event;
	queue_item_count++;
}

timer_t os_start_timer(float timeout) {
	timer_t timer;
	timer.id = ++timer_id;
	timer.type = 0;
	driver_timer_create(timer.id, (uint32_t)(timeout * 1000));
	return timer;
}

void os_cancel_timer(timer_t timer) {
	driver_timer_cancel(timer.id);
}

bool timer_equals(timer_t a, timer_t b) {
	return a.id == b.id;
}

void os_sleep(float timeout) {
	timer_t timer = os_start_timer(timeout);
	timer_t* timerEvent;
	do {
		event_t event = os_pull_event(EVENT_TIMER);
		timerEvent = (timer_t*)&event;
	} while (timerEvent->id != timer.id);
}

void os_shutdown(void) {

}

void os_reboot(void) {

}