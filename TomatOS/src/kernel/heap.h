#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include <stdint.h>
#include <stdbool.h>

void initialize_heap(size_t start, size_t size);

void* heap_allocate(size_t size);
void* heap_reallocate(void* ptr, size_t newsize);
bool heap_free(void* ptr);

#endif