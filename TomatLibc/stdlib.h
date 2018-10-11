#ifndef LIBC_STDLIB_H
#define LIBC_STDLIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    int atoi(const char* str);
    long atol(const char* str);
    long long atoll(const char* str);

    int abs(int n);
    int labs(long int n);

    void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*,const void*));
    void qsort(void* base, size_t num, size_t size, int (*compar)(const void*,const void*));

#ifdef __cplusplus
}
#endif
#endif