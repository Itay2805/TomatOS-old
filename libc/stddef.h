#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#define NULL (0)
#define UNUSED(x) (void)(x)

#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))

#if _MBCS
	#define offsetof(st, m) (size_t)(unsigned long)&(((st *)0)->m)
#else
	#define offsetof(st, m) __builtin_offsetof(st, m)
#endif

#if __cplusplus
extern "C" {
#endif

#if __cplusplus
}
#endif

#endif