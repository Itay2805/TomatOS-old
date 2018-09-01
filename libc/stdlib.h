#ifndef CLIB_STDLIB_H
#define CLIB_STDLIB_H

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

    void* malloc(size_t size);
    void* calloc(size_t num, size_t size);
    void* realloc(void* ptr, size_t newsize);
    void free(void* ptr);

#if __cplusplus
}
#endif

#endif