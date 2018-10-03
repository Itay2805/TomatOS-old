#include "process.h"

#include <stddef.h>
#include <string.h>

#include <core/process/syscall.h>
#include <core/process/perm.h>

#include <core/graphics/term.h>

#include <core/timer.h>

#include <boot/gdt/gdt.h>

//////////////////////////////////////////////////////////////
//// Static Variables
//////////////////////////////////////////////////////////////

#define PROCESS_MAX_STACK_SIZE (1024 * 1024 * 20)
#define ALIVE 0

static int uid = 0;
static bool can_update = false;
static process_t* processes = NULL;
static int updates;
static bool enabled = false;

static void pull_event(event_t* events, event_t* to);

//////////////////////////////////////////////////////////////
//// Alive process
//////////////////////////////////////////////////////////////

static void alive_wait_for_events(void) {
	// reset the status of the alive process, just in case
	processes[ALIVE].status = PROCESS_SUSPENDED;
	processes[ALIVE].started = true;
	while (true) {
		asm("nop"); // do nothing
		asm("hlt"); // do nothing
	}
}

static void initialize_alive(process_t* alive_process) {
	// the paging is only used when alive is waiting for events
	page_directory_t pd = (page_directory_t)paging_allocate_new_page();
	paging_init_directory(pd);

	// init the heap to use the kernel heap
	uintptr_t kernelHeapStart = (uintptr_t)((((size_t)&tomatkernel_end + 1024 * 1024) >> 12) << 12);
	heap_create(&alive_process->heap, kernelHeapStart);

	// only after we inited the heap we want to init the pd on the alive process
	alive_process->pd = pd;

	// this will be set to suspended since it is waiting for events technically
	alive_process->status = PROCESS_SUSPENDED;

	// the main method in this case is only used for when all processes
	// are waiting for events, and we must exit the interrupt and wait for
	// other events, the main of alive will just do nothing
	alive_process->main = alive_wait_for_events;

	// registers won't be really used
	// alive doesn't need events so no current event

	// set the stack to be the kernel stack
	// we will never really used it tbh
	alive_process->stack = (uintptr_t)(&tomatokernel_stack);

	// alive doesn't need events
	alive_process->events = NULL;

	// alive always runs as root
	alive_process->user = USER_ALIVE;

	// alive always has uid 0
	alive_process->uid = ALIVE;

	// doesn't really matter since it will never be activated
	alive_process->priority = 0;

	// background process
	alive_process->foreground = false;

	alive_process->started = true;
}

//////////////////////////////////////////////////////////////
//// syscalls
//////////////////////////////////////////////////////////////

static void syscall_start_alive(registers_t* regs) {
	if (enabled) return; // only runs once after everything is ready to run
	enabled = true;

	// enable paging (using the alive process for starters)
	paging_set(processes[ALIVE].pd);
	paging_enable();
	
	scheduler_update(regs, false);
}

static void syscall_pull_event(registers_t* regs) {
	process_t* process = process_get_running();
	if (process == NULL) {
		kpanic("[os] tried to call pull_event without a running process");
	}

	if (buf_len(process->events) > 0) {
		// we have an event, give it to the current process
		pull_event(process->events, &process->current_event);
		regs->eax = (uint32_t)&process->current_event;
	}
	else {
		// no events, save state, suspend and run the scheduler to find another process to run
		process->status = PROCESS_SUSPENDED;
		process->registers = *regs;
		scheduler_update(regs, false);
	}
}

static void syscall_queue_event(registers_t* regs) {
	event_t* event = (event_t*)regs->ebx;
	// verify pointer

	process_t* process;
	process_t* targetProcess;

	process = process_get_running();
	if (process == NULL) {
		kpanic("[os] called queue_event without a running process.");
	}

	uint32_t uid = regs->ecx;
	if (uid == 0) {
		// the target process is the current process
		targetProcess = process;
	}
	else {
		// get the target process
		targetProcess = process_get(uid);
	}

	if (targetProcess == NULL) {
		// error, target process must be running
		return;
	}
	
	// make sure we have permission over that process
	if (process != targetProcess && !has_permission(targetProcess->user, process->user)) {
		// error! to queue event on another process you must have root permission
		return;
	}

	// push the event
	buf_push(targetProcess->events, *event);
}

static void syscall_process_kill(registers_t* regs) {
	uint32_t uid = regs->ebx;
	
	process_t* process;
	process_t* current_process = process_get_running();
	
	// if uid is 0 then close the current process
	if(uid == 0) {
		process = current_process;
	}else {
		process = process_get(uid);
		// make sure we have permission to close the process
		if(has_permission(process->user, current_process->user)) {
			// TODO: Error (?)
			return;
		}
	}

	// SYSCALL_OS_KILL
	process_kill(regs, process);
}

//////////////////////////////////////////////////////////////
//// Implementation
//////////////////////////////////////////////////////////////

void process_init(void) {
	syscall_register(SYSCALL_START, syscall_start_alive);

	syscall_register(SYSCALL_OS_PULL_EVENT, syscall_pull_event);
	syscall_register(SYSCALL_OS_QUEUE_EVENT, syscall_queue_event);
	syscall_register(SYSCALL_OS_PROCESS_KILL, syscall_process_kill);

	process_t alive_process;
	initialize_alive(&alive_process);

	// allocate the sbuf manually (because currently there is still no alive process so the malloc will have no heap)
	size_t newCap = 16;
	size_t newSize = offsetof(BufHdr, buf) + newCap * sizeof(process_t);
	BufHdr* newHdr = (BufHdr*)heap_allocate(&alive_process.heap, newSize);
	newHdr->len = 1;
	newHdr->cap = newCap;
	processes = (process_t*)newHdr->buf;
	processes[0] = alive_process;
}

process_t* process_get(uint32_t uid) {
	for (process_t* process = processes; process < buf_end(processes); process++) {
		if (process->status == PROCESS_DEAD) continue;
		if (process->uid == uid) return process;
	}
	return NULL;
}

// may want to optimize this so it won't iterate over the running process over and over again....
process_t* process_get_running(void) {
	for (process_t* process = processes; process < buf_end(processes); process++) {
		if (process->status == PROCESS_RUNNING) return process;
	}
	return NULL;
}

void process_create(process_t* process, process_main_t main, int user,  bool foreground)
{
	memset(process, NULL, sizeof(process_t));

	process->uid = ++uid;

	// init paging
	process->pd = (page_directory_t)paging_allocate_new_page();
	paging_init_directory(process->pd);
	
	// the process will be located after the kernel and kernel heap (50MB)
	uintptr_t process_end = (uintptr_t)((size_t)&tomatkernel_end + 1024 * 1024 * 50 + process->process_size);
	
	// the stack will start 1MB after the process end
	// the stack will take a total of 20MB at max (to maybe allow the stack to grow in the future)
	uintptr_t process_stack = process_end + 1024 * 1024;
	uintptr_t process_stack_top = process_stack + PROCESS_MAX_STACK_SIZE;
	paging_map_range(process->pd, process_stack, PROCESS_STACK_SIZE);
	
	// the heap will be 1MB after the stack
	uintptr_t process_heap_start = process_stack_top + 1024 * 1024;

	// init heap
	size_t from = ((size_t)(process_heap_start) >> 12) << 12;
	process->heap.pd = process->pd;
	heap_create(&process->heap, (uintptr_t)from);
	
	process->status = PROCESS_SUSPENDED;
	process->stack = process_stack_top;
	process->events = NULL;
	process->started = false;

	process->user = user;

	process->main = main;
	process->foreground = foreground;
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

void process_kill(registers_t* regs, process_t* process) {
	can_update = false;

	// change process status
	bool wasRunning = false;
	if (process->status == PROCESS_RUNNING) {
		// we can not use this page directory anymore since it is going to 
		// get removed, we will use the ALIVE pd until we get are switching
		paging_set(processes[ALIVE].pd);
		wasRunning = true;
	}
	process->status = PROCESS_DEAD;

	// free all pages allocated for the process
	paging_free_directory(process->pd);

	// free the events queue
	buf_free(process->events);

	// free all the process timers
	timer_cancel(process->uid, 0);

	// delete processes related to window
	window_delete_for_process(process->uid);

	can_update = true;
	if (wasRunning) {
		// if this was the running process switch to another process
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
		
		memset(&newprocess->registers, 0, sizeof(registers_t));

		// this should set everything for running the process for the first time
		// there is a problem tho, right now when the process exited there is no return address...
		// maybe make it so the program must calls exit when finishing to run?
		newprocess->registers.esp = (uint32_t)newprocess->stack;
		newprocess->registers.ebp = (uint32_t)newprocess->stack;
		newprocess->registers.useresp = (uint32_t)newprocess->stack;
		newprocess->registers.eip = (uint32_t)newprocess->main;
		
		newprocess->registers.eflags = 0x202;
	}

	// set segments
	if (newprocess->user == USER_ALIVE) {
		newprocess->registers.ds = GDT_SEGMENT_DATA;
		newprocess->registers.cs = GDT_SEGMENT_CODE;
	}
	else {
		newprocess->registers.ds = GDT_SEGMENT_USER_DATA;
		newprocess->registers.cs = GDT_SEGMENT_USER_CODE;
	}

	// set the registers of the new process
	*currentregs = newprocess->registers;
	newprocess->status = PROCESS_RUNNING;
	paging_set(newprocess->pd);
}

void scheduler_update(registers_t* regs, bool fromTimer) {
	if (!enabled) return;
	if (!can_update) {
		// this should not matter since we disable interrupts
		if(fromTimer) updates++;
		return;
	}

	process_t* running_process = NULL;
	can_update = false;
	uint32_t count = 0;

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
					process->registers.eax = (uint32_t)&process->current_event;
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
