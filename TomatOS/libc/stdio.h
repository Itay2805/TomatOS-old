#ifndef LIBC_STDIO_HPP
#define LIBC_STDIO_HPP

#include <stdarg.h>

int vprintf(const char* format, va_list arg);
int printf(const char* format, ...);

#endif