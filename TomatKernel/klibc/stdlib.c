#include <stdlib.h>

#include <stddef.h>
#include <string.h>

// NOTE: These are only for the kernel to use and will allocate on the kernel heap, not the user heap
//		 The difference is that the user can only read from the kernel heap, not write to it

#include <kernel.h>
#include <core/memory/heap.h>

void* malloc(size_t size) {
	return heap_allocate(kheap(), size);
}

void* calloc(size_t num, size_t size) {
	void* data = heap_allocate(kheap(), num * size);
	memset(data, NULL, num * size);
	return NULL;
}

// TODO: Implement heap_size or something to get the size of a pointer
void* realloc(void* ptr, size_t newsize) {
	UNUSED(ptr);
	UNUSED(newsize);

	return NULL;
}

void free(void* ptr) {
	heap_free(kheap(), ptr);
}