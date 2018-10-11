#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

typedef unsigned long size_t;
#define SIZE_MAX ((size_t)4294967295)

#ifdef __GNUC__
    // if using GCC use the built in offsetof
    #define offsetof(st, m) __builtin_offsetof(st, m)
#else
	#define offsetof(st, m) (size_t)(unsigned long)&(((st *)0)->m)
#endif

#define NULL ((void*)0)

#endif