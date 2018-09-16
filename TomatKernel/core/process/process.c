#include "process.h"

#include <stddef.h>
#include <string.h>

#define PROCESS_MAX_STACK_SIZE (1024 * 1024 * 20)
#define ALIVE 0

static int uid = 0;
static bool can_update = false;
static process_t* processes = NULL;
static int updates;

void alive_wait_for_events(void) {
	// reset the status of the alive process, just in case
	processes[ALIVE].status = PROCESS_SUSPENDED;
	processes[ALIVE].started = true;
	while (true) {
		asm("nop"); // do nothing
	}
}

void process_init_alive(void) {
	process_t alive_process;

	// the paging is only used when alive is waiting for events
	alive_process.pd  = (page_directory_t)paging_allocate_new_page();
	paging_init_directory(alive_process.pd);

	// init the heap to use the kernel heap
	uintptr_t kernelHeapStart = (((size_t)&tomatkernel_end + 1024 * 1024) >> 12) << 12;
	heap_create(&alive_process.heap, kernelHeapStart);

	// this will be set to suspended since it is waiting for events technically
	alive_process.status = PROCESS_SUSPENDED;

	// the main method in this case is only used for when all processes
	// are waiting for events, and we must exit the interrupt and wait for
	// other events, the main of alive will just do nothing
	alive_process.main = alive_wait_for_events;

	// registers won't be really used
	// alive doesn't need events so no current event

	// set the stack to be the kernel stack
	// we will never really used it tbh
	alive_process.stack = tomatokernel_stack;

	// alive doesn't need events
	alive_process.events = NULL;

	// alive always runs as root
	alive_process.user = 0;

	// alive always has uid 0
	alive_process.uid = ALIVE;

	// doesn't really matter since it will never be activated
	alive_process.priority = 0;

	// background process
	alive_process.foreground = false;

	alive_process.started = true;

	buf_push(processes, alive_process);
}

process_t* process_get(uint32_t uid) {
	for (process_t* process = processes; process < buf_end(processes); process++) {
		if (process->uid == uid) return process;
	}
	return NULL;
}

process_t* process_get_running(void) {
	for (process_t* process = processes; process < buf_end(processes); process++) {
		if (process->status == PROCESS_RUNNING) return process;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////
// Process Memory Mapping
//////////////////////////////////////////////////////////////////////////////////////
// Virtual Addressing
//
// Page 0:
//		Always set to taken, this is just because otherwise it might cause a fault
//		if the page is allocated
//
// Page 1-255:
//		Unused (so the GDT will be simpler)
// 
// Page 256-kernel_end:
//		Kernel Pages, the user is not allowed to write or excute code in them
//
// 11520 Pages:
//		Kernel heap, the user is not allowed to write or excute code in it
//
// 1280 Pages: 
//		Unused, to use as buffer between kernel heap and program
//
// program_size Pages:
//		Where the program will be loaded to
//
// 256 Pages: 
//		Unused, to use as buffer between program and stack
//
// 5120 Pages:
//		Process stack (20MB)
//
// 256 Pages: 
//		Unused, to use as buffer between stack and heap
//
// Rest of the pages will be used as the heap of the process
//
//////////////////////////////////////////////////////////////////////////////////////

void process_create(process_t* context)
{
	memset(context, NULL, sizeof(process_t));

	context->uid = ++uid;

	// init paging
	context->pd = (page_directory_t)paging_allocate_new_page();
	paging_init_directory(context->pd);
	
	// the process will be located after the kernel and kernel heap (50MB)
	uintptr_t process_end = (uintptr_t)((size_t)&tomatkernel_end + 1024 * 1024 * 50 + context->process_size);
	
	// the stack will start 1MB after the process end
	// the stack will take a total of 20MB at max (to maybe allow the stack to grow in the future)
	uintptr_t process_stack = process_end + 1024 * 1024;
	uintptr_t process_stack_top = process_stack + PROCESS_MAX_STACK_SIZE;
	paging_map_range(context->pd, process_stack_top - PROCESS_STACK_SIZE, PROCESS_STACK_SIZE);
	
	// the heap will be 1MB after the stack
	uintptr_t process_heap_start = process_stack_top + 1024 * 1024;

	// init heap
	size_t from = ((size_t)(process_heap_start) >> 12) << 12;
	context->heap.pd = context->pd;
	heap_create(&context->heap, (uintptr_t)from);
	
	context->status = PROCESS_SUSPENDED;
	context->stack = process_stack_top;
	context->events = NULL;
	context->started = false;
}

void process_start(process_t* newprocess) {
	can_update = false;
	newprocess->started = false;
	newprocess->status = PROCESS_NORMAL;
	for (process_t* process = processes; process < buf_end(processes); process++) {
		if (process->status == PROCESS_DEAD) {
			*process = *newprocess;
			return;
		}
	}

	// needs to add new entry
	buf_push(processes, *newprocess);

	can_update = true;
}

void process_kill(registers_t* regs, uint32_t uid) {
	can_update = false;
	process_t* process = process_get(uid);
	bool wasRunning = false;
	if (process->status == PROCESS_RUNNING) {
		wasRunning = true;
	}
	process->status = PROCESS_DEAD;
	can_update = true;
	if (wasRunning) {
		scheduler_update(regs, false);
	}
}

static void pull_event(event_t* events, event_t* to) {
	BufHdr* buf = buf__hdr(events);
	buf->len--;
	*to = events[0];
	memmove(events, events + 1, buf->len * sizeof(event_t));
}

static void process_switch(registers_t* currentregs, process_t* current, process_t* newprocess) {
	if (current != NULL) {
		// save registers of current process
		current->registers = *currentregs;
		current->status = PROCESS_NORMAL;
	}

	if (!newprocess->started) {
		newprocess->started = true;
		newprocess->status = PROCESS_RUNNING;
		paging_set(newprocess->pd);
		
		memset(&newprocess->registers, 0, sizeof(registers_t));

		// this should set everything for running the process for the first time
		newprocess->registers.esp = newprocess->stack;
		newprocess->registers.ebp = newprocess->stack;
		newprocess->registers.useresp = newprocess->stack;
		newprocess->registers.eip = newprocess->main;
	}

	// set the registers of the new process
	*currentregs = newprocess->registers;
	newprocess->status = PROCESS_RUNNING;
	paging_set(newprocess->pd);
}

void process_pull_event(registers_t* regs, event_t* event) {
	// wants to get an event
	process_t* process = process_get_running();
	if (buf_len(process->events) > 0) {
		pull_event(process->events, event);
	}
	else {
		// no events, will switch to another process while waiting for events
		process->status = PROCESS_SUSPENDED;
		scheduler_update(regs, false);
	}
}

void scheduler_update(registers_t* regs, bool fromTimer) {
	if (!can_update) {
		// this should not matter since we disable interrupts
		if(fromTimer) updates++;
		return;
	}

	process_t* running_process = NULL;
	can_update = false;
	int count = 0;

	// we only want to update times if the update comes from the timer
	if (fromTimer) {
		int timePassed = updates * 10 + 10;
		updates = 0;

		for (process_t* process = processes; process < buf_end(processes); process++) {
			// update the time since last run

			if (process->status != PROCESS_DEAD && process->status != PROCESS_RUNNING) {
				process->suspended_time += timePassed;
				count++;
			}
			else if (process->status == PROCESS_RUNNING) {
				process->running_time += timePassed;
				running_process = process;
				count++;
			}

			if (process->status == PROCESS_SUSPENDED) {
				if (buf_len(process->events) != 0) {
					pull_event(process->events, &process->current_event);
					// the event is moved to eax since the project was suspended during a call to pull_event
					// and it will expect a return value for the pull_event when resuming
					process->registers.eax = &process->current_event;
					process->status = PROCESS_NORMAL;
				}
			}
		}
	}
		
	if (running_process == NULL) {
		int bestScore = 0;
		process_t* processWithBestScore = NULL;

		// no process is running! run one who waited the most
		for (process_t* process = processes; process < buf_end(processes); process++) {
			if (process->status != PROCESS_NORMAL) continue;
			if (!process->started) {
				processWithBestScore = process;
				break;
			}
			int score = process->suspended_time * (process->priority + 1);
			if (bestScore < score) {
				bestScore = score;
				processWithBestScore = process;
			}
		}

		if (processWithBestScore == NULL) {
			// all process are waiting for events... run the alive waiting loop
			processes[ALIVE].started = false;
			process_switch(regs, NULL, &processes[ALIVE]);
		}
		else {
			process_switch(regs, NULL, processWithBestScore);
		}
	}
	else {
		// there is a process running, check if should force-run another process
		if (running_process->running_time > (1000 / count) + (running_process->priority * 100)) {
			// we should do a switch

			int bestScore = 0;
			process_t* processWithBestScore = NULL;
			for (process_t* process = processes; process < buf_end(processes); process++) {
				if (process->status != PROCESS_NORMAL) continue;
				if (!process->started) {
					processWithBestScore = process;
					break;
				}
				int score = process->suspended_time * (process->priority + 1);
				if (bestScore < score) {
					bestScore = score;
					processWithBestScore = process;
				}
			}

			if (processWithBestScore == NULL) {
				// no other process to run, continue with current process
			}
			else {
				process_switch(regs, running_process, processWithBestScore);
			}
		}
	}
	// when the interrupt will exit it will set the registers 
	// meaning the switched process will be ran

	can_update = true;
}
