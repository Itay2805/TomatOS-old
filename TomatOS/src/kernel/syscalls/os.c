#include "os.h"

#include <stddef.h>

#include <tomato.h>

#include "../drivers/timer.h"

#include "../interrupt.h"
#include "../syscalls.h"
#include "../port.h"

static const char* version_str = "TomatOS - 2.0";

#include "term.h"

#define QUEUE_CAP 5

static event_t* array;
static int cap;
static int front;
static int rear;
static int count;

void os_kqueue_event(event_t event) {
	if (count == cap) {
		cap += QUEUE_CAP;
		array = realloc(array, sizeof(event_t) * cap);
	}
	array[++rear] = event;
	count++;
}

static void syscall_version(registers_t* regs) {
	regs->eax = (uint32_t)version_str;
}

static void syscall_pull_event(registers_t* regs) {
	uint32_t filter = regs->ecx;
	event_t* event = (event_t*)regs->ebx;
	while(true) {
		if (count == 0) {
			regs->eax = false;
			return;
		}
		*event = array[front++];
		if (front == cap) {
			front = 0;
		}
		count--;
		if (filter == TOMATO_EVENT_ALL || event->type & filter) {
			regs->eax = true;
			return;
		}
	} while (filter != TOMATO_EVENT_ALL && !(event->type & filter));
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
	cap = QUEUE_CAP;
	array = malloc(sizeof(event_t) * cap);
	front = 0;
	rear = -1;
	count = 0;

	register_syscall(TOMATO_SYSCALL_OS_VERSION, syscall_version);
	register_syscall(TOMATO_SYSCALL_OS_QUEUE_EVENT, syscall_queue_event);
	register_syscall(TOMATO_SYSCALL_OS_PULL_EVENT, syscall_pull_event);
	register_syscall(TOMATO_SYSCALL_OS_START_TIMER, syscall_create_timer);
	register_syscall(TOMATO_SYSCALL_OS_CANCEL_TIMER, syscall_cancel_timer);
}

#undef QUEUE_CAP
