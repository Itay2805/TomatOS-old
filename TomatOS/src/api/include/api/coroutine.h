#ifndef API_COROUTINE_H
#define API_COROUTINE_H

#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>

#define	COROUTINE_STATUS_RUNNING	0
#define COROUTINE_STATUS_NORMAL		1
#define COROUTINE_STATUS_SUSPENDED	2
#define COROUTINE_STATUS_DEAD		3

typedef void(*coroutine_func)(void* arg);

typedef struct coroutine_t {
	jmp_buf __caller;
	jmp_buf __yielder;
	coroutine_func __func;
	uint8_t __status;
	uintptr_t __stack;
	uintptr_t __prev_stack;
	bool __first;
} coroutine_t;

coroutine_t coroutine_create(coroutine_func func);
void* coroutine_resume(coroutine_t* coro, void* arg);
void* coroutine_yield(void* arg);
uint8_t coroutine_status(coroutine_t* coro);
coroutine_t* coroutine_current();

#endif
