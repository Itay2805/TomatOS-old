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

static void reverse(char str[], int length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char c = *(str + start);
		*(str + start) = *(str + end);
		*(str + end) = c;
		start++;
		end--;
	}
}


char* itoa(int32_t num, char* str, uint8_t base) {
	int i = 0;
	bool isNegative = false;

	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	if (num < 0 && base == 10) {
		isNegative = true;
		num = -num;
	}

	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	if (isNegative)
		str[i++] = '-';

	str[i] = '\0';
	reverse(str, i);
	return str;
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
