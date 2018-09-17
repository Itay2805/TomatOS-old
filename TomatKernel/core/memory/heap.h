#ifndef KERNEL_CORE_MEMORY_HEAP_H
#define KERNEL_CORE_MEMORY_HEAP_H

#include <stdbool.h>

#include <core/memory/paging.h>

#define HEAP_MAGIC 0xFACC0FF

typedef struct heap_block {
	bool allocated;
	struct heap_block* before;
	size_t size;
	uint32_t magic;
} heap_block_t;

typedef struct heap_context {
	page_directory_t pd;
	
	size_t size;
	size_t used_size;

	heap_block_t* top_block;

	heap_block_t* start;
	heap_block_t* first_free;
} heap_context_t;

#ifdef __cplusplus
extern "C" {
#endif

	void heap_init(void);

	void heap_create(heap_context_t* context, uintptr_t start_address);
	uintptr_t heap_allocate(heap_context_t* context, size_t size);
	void heap_free(heap_context_t* context, uintptr_t ptr);
	uintptr_t heap_reallocate(heap_context_t* context, uintptr_t ptr, size_t newSize);

#ifdef __cplusplus
}
#endif

#endif