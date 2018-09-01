#include <string.h>

size_t itoa(int32_t v, char* str, uint8_t radix) {
    if (v == 0 && radix != 16 && radix != 2) {
		str[0] = '0';
		str[1] = 0;
		return 1;
	}
	size_t count = 0;
	if (v < 0) {
		count++;
		str[0] = '-';
		v = -v;
	}
	return count + uitoa((uint32_t)v, str + count, radix);
}

size_t uitoa(uint32_t v, char* str, uint8_t radix) {
    if (v == 0 && radix != 16 && radix != 2) {
		str[0] = '0';
		str[1] = 0;
		return 1;
	}
	size_t num = 0;
	while (v) {
		if (v % radix >= 10) {
			str[num++] = 'A' + ((v % radix) - 10);
		}
		else {
			str[num++] = '0' + (v % radix);
		}
		v /= radix;
	}
    str[num] = 0;
	strrev(str);
	return num;
}

char* strrchr(const char *s, int c) {
	const char* ret = 0;
	do {
		if (*s == (char)c) {
			ret = s;
		}
	} while (*s++);
	return ret;
}

char* strrev(char* str) {
    char* front = str;
    char* back = str + strlen(str) - 1;
    for(; front < back; front++, back--) {
        char c = *front;
        *front = *back;
        *back = c;
    }
    return str;
}

size_t strlen(const char *s) {
	const char *p = s;
	while (*s) ++s;
	return s - p;
}

int strcmp(const char * s1, const char * s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++, s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int memcmp(const void * s1, const void * s2, size_t n) {
	const unsigned char *p1 = s1, *p2 = s2;
	while (n--) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		} else {
			p1++, p2++;
		}
	}
	return 0;
}

void* memcpy(void *dest, const void *src, size_t n) {
	char *dp = dest;
	const char *sp = src;
	while (n--) {
		*dp++ = *sp++;
	}
	return dest;
}

void* memmove(void *dest, const void *src, size_t n) {
#ifndef _MBCS
	unsigned char tmp[n];
	memcpy(tmp, src, n);
	memcpy(dest, tmp, n);
#else
	memcpy(dest, src, n);
#endif
	return dest;
}

void* memset(void *s, int c, size_t n) {
	unsigned char* p = s;
	while (n--) {
		*p++ = (unsigned char)c;
	}
	return s;
}
