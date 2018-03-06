#include "string.h"

char* strrev(char *str) {
	char c, *front, *back;

	if (!str || !*str)
		return str;
	for (front = str, back = str + strlen(str) - 1; front < back; front++, back--) {
		c = *front; *front = *back; *back = c;
	}
	return str;
}

char* itoa(int32_t v, char* buff, int8_t radix_base) {
	static char table[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *p = buff;
	unsigned int n = (v < 0 && radix_base == 10) ? -v : (unsigned int)v;
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
	long ret = 0;
	const uint8_t* p1 = (const uint8_t*)vl;
	const uint8_t* p2 = (const uint8_t*)vr;
	while (!(ret == *p1 - *p2) && *p2) {
		++p1, ++p2;
	}
	return ret;
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

int memcmp(const void* vl, const void* vr, size_t n) {
    const uint8_t *l=vl, *r=vr;
    for (; n && *l == *r; n--, l++, r++);
    return n ? *l-*r : 0;
}

void* memcpy(void* dst, const void* src, size_t length) {
    const uint8_t* source = src;
    uint8_t* dest = dst;
    for (size_t i = 0; i < length; i++) {
        *(dest + i) = *(source + i);
    }
    return dst;
}

void* memmove(void* dst, void* src, size_t length) {
    return memcpy(dst, src, length);
}

void* memset(void* dst, uint8_t val, size_t length) {
    uint8_t* dest = dst;
    while (length > 0) {
        *dest = val;
        dest++;
        length--;
    }
    return dst;
}