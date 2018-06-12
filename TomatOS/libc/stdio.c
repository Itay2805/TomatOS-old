#include "stdio.h"

#include <tomato.h>

int printf(const char* format, ...) {
	va_list va;
	va_start(va, format);
	int i = vprintf(format, va);
	va_end(va);
	return i;
}

int vprintf(const char* format, va_list arg) {
	char charbuf[2];
	charbuf[1] = 0;

	char numbuf[20];

	int count = 0;
	bool specifier = false;
	while(*format) {
		if (specifier) {
			switch (*format) {
				case 'x': {
					int32_t num = va_arg(arg, int32_t);
					itoa(num, numbuf, 16);
					count += strlen(numbuf);
					tomato_term_write(numbuf);
				} break;
				case 'i':
				case 'd': {
					int32_t num = va_arg(arg, int32_t);
					itoa(num, numbuf, 10);
					count += strlen(numbuf);
					tomato_term_write(numbuf);
				} break;
				case 's': {
					char* str = va_arg(arg, char*);
					count += strlen(str);
					tomato_term_write(str);
				} break;
				case 'p': {
					uint32_t ptr = va_arg(arg, uint32_t);
					itoa(ptr, numbuf, 16);
					count += strlen(numbuf);
					tomato_term_write(numbuf);
				} break;
				case '%': {
					charbuf[0] = '%';
					tomato_term_write(charbuf);
					count++;
				} break;
			}
			specifier = false;
		}
		else {
			if (*format == '%') {
				specifier = true;
			}
			else {
				charbuf[0] = *format;
				tomato_term_write(charbuf);
				count++;
			}
		}
		format++;
	}
	
	return count;
}
