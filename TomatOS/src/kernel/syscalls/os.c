#include "os.h"

#include <stddef.h>

#include <tomato.h>

#include "../drivers/timer.h"

#include "../interrupt.h"
#include "../syscalls.h"
#include "../port.h"
#include "../heap.h"

static const char* version_str = "TomatOS - 2.0";

#include "term.h"

#define QUEUE_CAP 5

static event_t* array;
static int cap;
static int front;
static int rear;
static int count;

// for syncing stuff we need to lock the queue
static bool locked = false;

void os_kqueue_event(event_t event) {
	locked = true;
	
	if (count == cap) {
		cap += QUEUE_CAP;
		array = heap_reallocate(array, sizeof(event_t) * cap);
	}
	if (rear == cap - 1) {
		rear = -1;
	}
	array[++rear] = event;
	count++;

	locked = false;
}

static void syscall_version(registers_t* regs) {
	regs->eax = (uint32_t)version_str;
}

static void syscall_pull_event(registers_t* regs) {
	// if the queue is locked, say there is no event
	if (locked) {
		regs->eax = 0;
		return;
	}

	uint32_t filter = regs->ecx;
	event_t* event = (event_t*)regs->ebx;
	while (true) {
		// if queue gets locked while we iterate it return 
		// also if there are no items in the queue return
		if (locked || count == 0) {
			regs->eax = 0;
			return;
		}
		*event = array[front++];
		if (front == cap) {
			front = 0;
		}
		count--;
		if (filter == TOMATO_EVENT_ALL || event->type == filter) {
			regs->eax = count + 1;
			return;
		}
	}


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
	array = heap_allocate(sizeof(event_t) * cap);
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
