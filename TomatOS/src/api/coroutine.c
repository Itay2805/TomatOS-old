#include "include/api/coroutine.h"

#include <memory.h>

#define COROUTINE_STACK_SIZE 1024*1024

static coroutine_t* current;
static void* yieldarg;
static void* funcparam;

coroutine_t coroutine_create(coroutine_func func) {
	coroutine_t coro;
	coro.__status = COROUTINE_STATUS_SUSPENDED;
	coro.__func = func;
	coro.__first = true;
	coro.__stack = nullptr;
	coro.__prev_stack = nullptr;
	return coro;
}

void* coroutine_resume(coroutine_t* coro, void* arg) {
	// save old coroutine reference
	coroutine_t* old = current;
	// set the current coroutine to nomral status (since it called a resume)
	if (current != nullptr) {
		current->__status = COROUTINE_STATUS_NORMAL;
	}
	current = coro;
	// set the coroutine to running status
	coro->__status = COROUTINE_STATUS_RUNNING;
	// allocate a stack if is not allocated
	if (coro->__stack == nullptr) {
		coro->__stack = malloc(COROUTINE_STACK_SIZE);
	}
	if (!setjmp(current->__caller)) {
		// starting coroutine
		funcparam = arg;
		
		// save current stack
		uintptr_t prev_stack = 0;
#ifndef VISUAL_STUDIO
		asm("movl %0, %%esp" : "=r"(prev_stack) : );
#endif
		current->__prev_stack = prev_stack;
		if (current->__first) {
			// set new stack
			// we leave space for the longjmp
			uintptr_t stack = current->__stack + COROUTINE_STACK_SIZE;
#ifndef VISUAL_STUDIO
			asm("movl %%esp, %0" : : "r"(stack));
#endif
			// if we just started the coroutine, just call the function normally
			current->__first = false;
			current->__func(funcparam);
		}
		else {
			// otherwise longjmp to the yielder
			// the stack is restored automatically
			longjmp(current->__yielder, 1);
		}

		// restore the old stack, we use funcparam since it is static and not on the stack, might wanna put it in another variable
		// for better clearity 
		funcparam = current->__prev_stack;
#ifndef VISUAL_STUDIO
		asm("movl %%esp, %0" : : "r"(funcparam));
#endif
		// free the stack
		free(coro->__stack);
		coro->__stack = nullptr;
		// set the status to dead
		coro->__status = COROUTINE_STATUS_DEAD;
		// restore old coroutine
		current = old;
		if (current != nullptr) {
			current->__status = COROUTINE_STATUS_RUNNING;
		}
		return 0;
	}
	else {
		// coroutine yielded
		// stack is restored from longjmp
		// set yielder to suspended
		current->__status = COROUTINE_STATUS_SUSPENDED;
		// pass control back to the old coroutine
		current = old;
		if (current != nullptr) {
			current->__status = COROUTINE_STATUS_RUNNING;
		}
		return yieldarg;
	}
}

void* coroutine_yield(void* arg) {
	yieldarg = arg;
	if (!setjmp(current->__yielder)) {
		longjmp(current->__caller, 1);
		return 0;
	}
	else {
		return funcparam;
	}
}

uint8_t coroutine_status(coroutine_t* coro) {
	return coro->__status;
}

coroutine_t* coroutine_current() {
	return current;
}

#undef COROUTINE_STACK_SIZE
