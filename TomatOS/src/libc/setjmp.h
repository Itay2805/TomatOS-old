#ifndef LIBC_SETJMP_H
#define LIBC_SETJMP_H

#include <stdint.h>

typedef uint32_t jmp_buf[20];

extern int __builtin_setjmp(jmp_buf* buf);
extern void __builtin_longjmp(jmp_buf* buf, int m);

#define setjmp __builtin_setjmp
#define longjmp __builtin_longjmp

#endif