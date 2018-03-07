#include "include/api/os.h"

#include <memory.h>
#include <string.h>

#include <api/term.h>
#include <api/color.h>

#define API_OS_QUEUE_INC 5

event_t* __event_queue;
int __queue_cap;
volatile int __queue_size;
const char* __label;

event_t __pull_event(void) {
	// wait for next event, need a better way
	while (__queue_size == 0) {
		__asm__("");
	}
	// get the event
	event_t event = __event_queue[__queue_size];
	__queue_size--;
	return event;
}

void __queue_event(event_t event) {
	__queue_size++;
	if (__queue_cap >= __queue_size) {
		__queue_cap += API_OS_QUEUE_INC;
		__event_queue = realloc(__event_queue, __queue_cap);
	}
	__event_queue[__queue_size] = event;
}

void kernel_os_init(void) {
	__queue_size = 0;
	__queue_cap = 0;
	__event_queue = nullptr;

	// TODO: somehow load label
	__label = "<unlabeled>";
}

const char* os_version(void) {
	return "TomatOS";
}

const char* os_get_computer_label(void) {
	return __label;
}

void os_set_computer_label(char* name) {
	__label = name;
	// save label
}

event_t os_pull_event_raw(uint32_t filter) {
	event_t event;
	do {
		event = __pull_event();
	} while(filter != EVENT_ALL && event.type == filter);
	return event;
}

event_t os_pull_event(uint32_t filter) {
	event_t event = os_pull_event_raw(filter);
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
		term_write(exception->msg);
	}
	return event;
}

void os_queue_event(event_t event) {
	__queue_event(event);
}

timer_t os_start_timer(uint32_t timeout) {
	timer_t timer;
	timer.id = timeout;
	timer.type = 0;
	return timer;
}

void os_cancel_timer(timer_t timer) {

}

bool timer_equals(timer_t a, timer_t b) {
	return a.id == b.id;
}

void os_sleep(uint32_t timeout) {

}

void os_shutdown(void) {

}

void os_reboot(void) {

}