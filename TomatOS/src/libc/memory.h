#ifndef LIBC_MEMORY_H
#define LIBC_MEMORY_H

#include "stdint.h"
#include "stdbool.h"

#define nullptr ((void*)0)

void kernel_memory_init(void);

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void* ptr, size_t newsize);
bool free(void* ptr);

#endif