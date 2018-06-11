#include "string.h"

// TODO: make so it will only have inlined assembly
#include <syscall.h>

void* malloc(size_t size) {
	return (void*)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_ALLOCATE, (uint32_t)size);
}

void* realloc(void* ptr, size_t newsize) {
	return (void*)tomato_syscall_2p(TOMATO_SYSCALL_HEAP_REALLOCATE, (uint32_t)ptr, (uint32_t)newsize);
}

int free(void* ptr) {
	return (bool)tomato_syscall_1p(TOMATO_SYSCALL_HEAP_FREE, (uint32_t)ptr);
}

char* strrev(char *str) {
	char c, *front, *back;

	if (!str || !*str)
		return str;
	for (front = str, back = str + strlen(str) - 1; front < back; front++, back--) {
		c = *front; *front = *back; *back = c;
	}
	return str;
}

char* itoa(int32_t v, char* buff, uint8_t radix_base) {
	static char table[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *p = buff;
	uint32_t n = (v < 0 && radix_base == 10) ? (uint32_t)(-v) : (uint32_t)v;
	while (n >= radix_base) {
		*p++ = table[n%radix_base];
		n /= radix_base;
	}
	*p++ = table[n];
	if (v < 0 && radix_base == 10) *p++ = '-';
	*p = '\0';
	return strrev(buff);
}

int strcmp(const char* vl, const char* vr) {
	int l1 = strlen(vl);
	int l2 = strlen(vr);
	if (l1 == 0 && l2 != 0) {
		return 1;
	}
	else if (l1 != 0 && l2 == 0) {
		return -1;
	}
	int min = l1 > l2 ? l2: l1;
	return memcmp(vl, vr, min);
}

char* strcpy(char* dst, const char* src) {
	char* ret = dst;
	while (*src) {
		*dst++ = *src;
	}
	*dst = 0;
	return ret;
}

size_t strlen(const char* str) {
	size_t cnt = 0;
	if (!str) return 0;
	while (*str != 0) {
		cnt++;
		str++;
	}
	return cnt;
}

int strncmp(const char* s1, const char* s2, size_t n) {
	int l1 = strlen(s1);
	int l2 = strlen(s2);
	if (l1 == 0 && l2 != 0) {
		return 1;
	}
	else if (l1 != 0 && l2 == 0) {
		return -1;
	}
	int min = l1 > l2 ? l2 : l1;
	min = min < n ? min : n;
	return memcmp(s1, s2, min);
}

int memcmp(const void* vl, const void* vr, size_t n) {
	const uint8_t *l = vl, *r = vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l - *r : 0;
}

void* memcpy(void* dst, const void* src, size_t length) {
	const uint8_t* source = src;
	uint8_t* dest = dst;
	while (length--) {
		*dest = *source;
		dest++;
		source++;
	}
	return dst;
}

void* memmove(void* dst, void* src, size_t length) {
	return memcpy(dst, src, length);
}

void* memset(void* dst, uint8_t val, size_t length) {
	uint8_t* dest = (uint8_t*)dst;
	while (length--) {
		*dest++ = val;
	}
	return dst;
}
