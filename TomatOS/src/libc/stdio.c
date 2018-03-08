#include "stdio.h"

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include <api/term.h>

// really not complete

int printf(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	bool format = false;
	char buf[2];
	buf[1] = 0;
	int printed = 0;
	char intBuf[50];
	while (*fmt) {
		char c = *fmt;
		fmt++;

		if(format) {
			switch (c) {
				case '%': {
					buf[0] = c;
					term_write(buf);
					printed++;
				}
				break;
				case 's': {
					char* str = va_arg(va, char*);
					term_write(str);
					printed += strlen(str);
				}
				break;
				case 'i':
				case 'd': {
					int number = va_arg(va, int);
					itoa(number, intBuf, 10);
					term_write(intBuf);
					printed += strlen(intBuf);
				}
				break;
				case 'u': {
					unsigned int number = va_arg(va, unsigned int);
					itoa(number, intBuf, 10);
					term_write(intBuf);
					printed += strlen(intBuf);
				}
				break;
				case 'o': {
					unsigned int number = va_arg(va, unsigned int);
					itoa(number, intBuf, 8);
					term_write(intBuf);
					printed += strlen(intBuf);
				}
				break;
				case 'x': {
					unsigned int number = va_arg(va, unsigned int);
					itoa(number, intBuf, 16);
					term_write(intBuf);
					printed += strlen(intBuf);
				}
				break;
				case 'c': {
					char c = va_arg(va, char);
					buf[0] = c;
					term_write(buf);
					printed++;
				}
				break;
				case 'p': {
					size_t ptr = va_arg(va, size_t);
					itoa(ptr, intBuf, 16);
					term_write(intBuf);
					printed += strlen(intBuf);
				}
				break;
				case 'n': {
					int* n = va_arg(va, int*);
					*n = printed;
				}
			}
			format = false;
		}else  if (c == '%') {
			format = true;
		} else {
			buf[0] = c;
			term_write(buf);
			printed++;
		}
	}
	va_end(va);
	return printed;
}
