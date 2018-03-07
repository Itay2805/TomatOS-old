#ifndef LIBC_STDARG_H
#define LIBC_STDARG_H

#define va_list char*
#define va_start(ap, arg) (ap = (va_list)&arg + sizeof(arg))
#define va_arg(ap, t) (*(t*)((ap + sizeof(t)) - sizeof(t)))
#define va_end(ap) (ap = (va_list)0)

#endif // !LIBC_STDARG_H
