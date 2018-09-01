#ifndef KERNEL_CORE_PROCESS_H
#define KERNEL_CORE_PROCESS_H

#include <core/memory/paging.h>
#include <core/memory/heap.h>

#include <kernel.h>

#define PROCESS_STACK_SIZE (1024 * 1024 * 2)

typedef enum process_status {
	PROCESS_RUNNING,
	PROCESS_NORMAL,
	PROCESS_SUSPENDED,
	PROCESS_DEAD
} process_status_t;

typedef void(*process_main_t)();

typedef struct process {
	page_directory_t pd;
	heap_context_t heap;

	uintptr_t process_size;
	uintptr_t process_end;

	process_status_t status;
	process_main_t main;

	uintptr_t stack;

	uint32_t uid;
} process_t;

#ifdef __cplusplus
extern "C" {
#endif
	
	void process_create(process_t* context);

#ifdef __cplusplus
}
#endif


#endif