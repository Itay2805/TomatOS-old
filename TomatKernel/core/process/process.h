#ifndef KERNEL_CORE_PROCESS_H
#define KERNEL_CORE_PROCESS_H

#include <core/memory/paging.h>
#include <core/memory/heap.h>
#include <core/sbuf.h>

#include <core/process/event.h>

#include <kernel.h>

#include <stdbool.h>

#define PROCESS_STACK_SIZE (1024 * 1024 * 2)

typedef enum process_status {
	PROCESS_RUNNING,
	PROCESS_NORMAL,
	PROCESS_SUSPENDED,
	PROCESS_DEAD
} process_status_t;

typedef enum event_type {
	EVENT_ALL,
	EVENT_TIMER,
} event_type_t;

typedef struct event {
	event_type_t type;
	uint32_t data[4];
} event_t;

typedef void(*process_main_t)();

typedef struct process {
	page_directory_t pd;
	heap_context_t heap;

	uintptr_t process_size;
	uintptr_t process_end;

	process_status_t status;
	process_main_t main;

	registers_t registers;
	event_t current_event;
	uintptr_t stack;

	event_t* events;

	uint32_t user;
	uint32_t uid;

	uint32_t suspended_time;
	uint32_t running_time;
	uint8_t priority;
	bool foreground;
	bool started;
} process_t;

#ifdef __cplusplus
extern "C" {
#endif
	
	void process_init_alive(void);

	void process_create(process_t* process);
	
	void process_start(process_t* process);
	
	void process_kill(registers_t* regs, uint32_t uid);

	process_t* process_get(uint32_t uid);

	process_t* process_get_running(void);

	// runs when a process wants to get an event
	void process_pull_event(registers_t* regs, event_t* event);
	
	// runs every 10ms
	void scheduler_update(registers_t* regs, bool fromTimer);
		
#ifdef __cplusplus
}
#endif


#endif