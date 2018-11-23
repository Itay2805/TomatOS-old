#ifndef KERNEL_CORE_PROCESS_H
#define KERNEL_CORE_PROCESS_H

#include <core/memory/paging.h>
#include <core/memory/heap.h>
#include <core/sbuf.h>

#include <kernel.h>

#include <stdbool.h>

#define PROCESS_STACK_SIZE (1024 * 1024 * 2)

typedef enum process_status {
	PROCESS_RUNNING,
	PROCESS_NORMAL,
	PROCESS_SUSPENDED,
	PROCESS_DEAD
} process_status_t;

// TODO: Move this to the TomatLib side so it will be consistant 
typedef enum event_type {
	EVENT_ALL,
	EVENT_CHAR,
	EVENT_KEY,
	EVENT_KEY_UP,
	EVENT_TIMER,
	EVENT_TERMINATE,
	EVENT_MOUSE_CLICK,
	EVENT_MOUSE_UP,
	EVENT_MOUSE_SCROLL,
	EVENT_MOUSE_DRAG,
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
	bool core;
	bool foreground;
	bool started;
} process_t;

#ifdef __cplusplus
extern "C" {
#endif
	
	void process_init(void);

	void process_create(process_t* context, process_main_t main, int user, bool foreground);

	void process_start(process_t* process);
		
	void process_kill(registers_t* regs, process_t* process);

	process_t* process_get_core_process(void);

	process_t* process_get(uint32_t uid);

	process_t* process_get_running(void);

	// runs every 10ms
	void scheduler_update(registers_t* regs, bool fromTimer);
		
#ifdef __cplusplus
}
#endif


#endif