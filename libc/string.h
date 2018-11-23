#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    size_t itoa(int32_t, char* str, uint8_t radix);
    size_t uitoa(uint32_t, char* str, uint8_t radix);

    char* strrev(char* str);
	size_t strlen(const char *s);
	int strcmp(const char * s1, const char * s2);
	char* strrchr(const char *s, int c);

	int memcmp(const void * s1, const void * s2, size_t n);
	void* memcpy(void *dest, const void *src, size_t n);
	void* memmove(void *dest, const void *src, size_t n);
	void* memset(void *s, int c, size_t n);

#ifdef __cplusplus
}
#endif

#endif