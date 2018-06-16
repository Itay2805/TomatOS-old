#include "string.h"

// TODO: make so it will only have inlined assembly
#include <syscall.h>

char* strrev(char *str) {
	char c, *front, *back;

	if (!str || !*str)
		return str;
	for (front = str, back = str + strlen(str) - 1; front < back; front++, back--) {
		c = *front; *front = *back; *back = c;
	}
	return str;
}

size_t itoa(int32_t v, char* buffer, uint8_t base) {
	if (v == 0 && base != 16 && base != 2) {
		buffer[0] = '0';
		buffer[1] = 0;
		return 1;
	}
	size_t count = 0;
	if (v < 0) {
		count++;
		buffer[0] = '-';
		v = -v;
	}
	return count + uitoa((uint32_t)v, buffer + count, base);
}

size_t uitoa(uint32_t v, char* buffer, uint8_t base) {
	if (v == 0 && base != 16 && base != 2) {
		buffer[0] = '0';
		buffer[1] = 0;
		return 1;
	}
	size_t num = 0;
	while (v > 0) {
		buffer[num++] = '0' + (v % base);
		v /= base;
	}
	strrev(buffer);
	return num;
}

int strcmp(const char * s1, const char * s2) {
	for (; *s1 == *s2; ++s1, ++s2) {
		if (*s1 == 0) return 0;
	}
	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
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
