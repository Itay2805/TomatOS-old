#ifndef LIBC_STDARG_HPP
#define LIBC_STDARG_HPP

typedef char* va_list;

#define __va_argsiz(t) (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, pN) ((ap) = ((va_list) (&pN) + __va_argsiz(pN)))
#define va_arg(ap, t) (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))
#define va_end(ap) ((void)0)

#endif