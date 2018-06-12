#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#include <stdint.h>

#define NULL (0)
#define UNUSED(x) (void)(x)
#define STR(x) #x

#define offsetof(type, element) ((unsigned long)&(((type *)0)->element))

#if __cplusplus
extern "C" {
#endif

	void* malloc(size_t size);
	void* realloc(void* ptr, size_t newsize);
	int free(void* ptr);

#if __cplusplus
}
#endif

#endif
