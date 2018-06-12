#include "stddef.h"

#include <tomato.h>

void* malloc(size_t size) {
	return (void*)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_ALLOCATE, (uint32_t)size);
}

void* realloc(void* ptr, size_t newsize) {
	return (void*)tomato_syscall_2p(TOMATO_SYSCALL_HEAP_REALLOCATE, (uint32_t)ptr, (uint32_t)newsize);
}

int free(void* ptr) {
	return (bool)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_FREE, (uint32_t)ptr);
}
