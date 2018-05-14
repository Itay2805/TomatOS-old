#include "os.h"

#include <stddef.h>

#include <tomato.h>

#include "../drivers/timer.h"

#include "../interrupt.h"
#include "../syscalls.h"
#include "../port.h"

static const char* version_str = "TomatOS - 2.0";

#include "term.h"

static struct queue_t {
	event_t* data;
	int cap;
	int front;
	int rear;
	int count;
} events;

void os_kqueue_event(event_t event) {
	if (events.cap == events.count) {
		events.data = realloc(events.data, events.cap + 5);
	}
	if (events.rear == events.cap - 1) {
		events.rear = -1;
	}
	events.data[++events.rear] = event;
	events.count++;
}

static void syscall_version(registers_t* regs) {
	regs->eax = (uint32_t)version_str;
}

static void syscall_queue_event(registers_t* regs) {
	event_t* event = (event_t*)regs->ebx;
	os_kqueue_event(*event);
}

static void syscall_create_timer(registers_t* regs) {
	uint32_t id = driver_start_timer(regs->ebx);
	regs->eax = id;
}

static void syscall_cancel_timer(registers_t* regs) {
	driver_cancel_timer(regs->ebx);
}

void syscall_os_init(void) {
	events.count = 0;
	events.front = 0;
	events.rear = -1;
	events.cap = 5;
	events.data = malloc(sizeof(event_t) * 5);

	register_syscall(TOMATO_SYSCALL_OS_VERSION, syscall_version);
	register_syscall(TOMATO_SYSCALL_OS_QUEUE_EVENT, syscall_queue_event);
	register_syscall(TOMATO_SYSCALL_OS_START_TIMER, syscall_create_timer);
	register_syscall(TOMATO_SYSCALL_OS_CANCEL_TIMER, syscall_cancel_timer);
}
