#include "process.h"

#include <stddef.h>
#include <string.h>

#define PROCESS_MAX_STACK_SIZE (1024 * 1024 * 20)

static int uid = 0;

void process_create(process_t* context)
{
	memset(context, NULL, sizeof(process_t));

	context->uid = uid++;

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
}
