#ifndef TOMATO_API_COROUTINE_HPP
#define TOMATO_API_COROUTINE_HPP

#include <setjmp.h>
#include <string.h>

namespace Tomato {

#define COROUTINE_STACK_SIZE 1024*1024

	class Coroutine {
	public:
		enum Status {
			RUNNING,
			NORMAL,
			SUSPENDED,
			DEAD
		};

		typedef void(*CoroutineFunc)(void* arg);

	private:
		// @TODO: fix "whose type is from an anonymous namespace"
		jmp_buf caller;
		jmp_buf yielder;
		CoroutineFunc func;
		Status status;
		uintptr_t stack;
		uintptr_t prevStack;
		bool first;

	public:
		Coroutine(CoroutineFunc func);
		~Coroutine();

		template<typename Argument = void*, typename Return = void*>
		Return Resume(void* arg = nullptr) {
			// save old coroutine reference
			Coroutine* old = current;
			// set the current coroutine to normal status
			if (current != nullptr) {
				current->status = Status::NORMAL;
			}
			current = this;
			// set the coroutine to running status
			this->status = Status::RUNNING;
			// allocate a stack if is not allocated
			if (this->stack == nullptr) {
				this->stack = (uintptr_t)malloc(COROUTINE_STACK_SIZE);
			}
			if (!setjmp(current->caller)) {
				// starting coroutine

				// save current stack
				uintptr_t prevStack = 0;
				asm("movl %%esp, %0" : "=r"(prevStack) : );
				current->prevStack = prevStack;

				// is this the first time the coroutine resumes?
				if (current->first) {
					// if set the stack and call the function
					uintptr_t stack = current->stack + COROUTINE_STACK_SIZE;
					asm("movl %0, %%esp" : : "r"(stack));
					current->first = false;
					current->func(arg);
				}
				else {
					// otherwise longjmp to the yielder
					// the stack is restored automatically
					yieldarg = arg;
					longjmp(current->yielder, 1);
				}

				// restore the old stack
				arg = current->prevStack;
				asm("movl %0, %%esp" : : "r"(arg));

				// free the stack
				free(this->stack);
				this->stack = nullptr;
				// set the status to dead
				this->status = Status::DEAD;
				// restore the old coroutine
				current = old;
				if (current != nullptr) {
					current->status = Status::RUNNING;
				}
				return (Return)0;
			}
			else {
				// coroutine yielded
				// stack is restored from longjmp
				// set yielder to suspended
				current->status = Status::SUSPENDED;
				current = old;
				if (current != nullptr) {
					current->status = Status::RUNNING;
				}
				return (Return)yieldarg;
			}
		}

		Status GetStatus();

	private:
		static Coroutine * current;
		static void* yieldarg;

	public:
		static Coroutine* Current();

		template<typename Return = void*, typename Argument = void*>
		static Return Yield(Argument arg = nullptr) {
			yieldarg = (void*)arg;
			if (!setjmp(current->yielder)) {
				longjmp(current->caller, 1);
				return 0;
			}
			else {
				return (Return)yieldarg;
			}
		}

	};

}

#endif
