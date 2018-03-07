#ifndef API_OS_H
#define API_OS_H

#include <stdint.h>
#include <stdbool.h>

#define EVENT_ALL			0
#define EVENT_TERMINATE		1
#define EVENT_TIMER			2
#define EVENT_EXCEPTION		3

typedef struct {
	uint32_t type;
	size_t data[4];
} event_t;

typedef struct {
	// if from event it is EVENT_TIMER otherwise it is NULL
	// this is so you can easily convert the event_t to a timer_t 
	uint32_t type;
	uint32_t id;
} timer_t;

typedef struct {
	uint32_t type;
	uint32_t code;
	const char* msg;
} exception_t;

bool timer_equals(timer_t a, timer_t b);

void kernel_os_init(void);

const char* os_version(void);
const char* os_get_computer_label(void);
void os_set_computer_label(char* name);

event_t os_pull_event_raw(uint32_t filter);
event_t os_pull_event(uint32_t filter);
void os_queue_event(event_t event);

timer_t os_start_timer(uint32_t timeout);
void os_cancel_timer(timer_t timer);
void os_sleep(uint32_t timeout);



void os_shutdown(void);
void os_reboot(void);

#endif