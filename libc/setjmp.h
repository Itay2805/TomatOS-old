#ifndef LIBC_SETJMP_H
#define LIBC_SETJMP_H

#include <stdint.h>

typedef struct {
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;
	uint32_t esp;
	uint32_t eip;
} jmp_buf[1];

#ifdef __cplusplus
extern "C" {
#endif

	extern int setjmp(jmp_buf env);
	extern void longjmp(jmp_buf env, int value);

#ifdef __cplusplus
}
#endif

#endif