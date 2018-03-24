#ifndef LIBC_STDARG_H
#define LIBC_STDARG_H

#ifndef VISUAL_STUDIO
typedef __builtin_va_list va_list; 
#else
typedef char* va_list;
#endif

#define __va_argsiz(t) (((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_start(ap, pN) ((ap) = ((va_list) __builtin_next_arg(pN)))
#define va_end(ap)	((void)0)
#define va_arg(ap, t) (((ap) = (ap) + __va_argsiz(t)), *((t*) (void*) ((ap) - __va_argsiz(t))))

#endif // !LIBC_STDARG_H
