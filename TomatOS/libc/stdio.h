#ifndef LIBC_STDIO_HPP
#define LIBC_STDIO_HPP

#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif

	int vprintf(const char* format, va_list arg);
	int printf(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif