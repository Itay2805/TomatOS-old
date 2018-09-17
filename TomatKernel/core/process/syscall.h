#ifndef KERNEL_CORE_PROCESS_SYSCALL_H
#define KERNEL_CORE_PROCESS_SYSCALL_H

#include <kernel.h>

#define CHECK_POINTER(ptr) ((ptr) > &tomatkernel_end + 1024 * 1024 * 50)

#if __cplusplus
extern "C" {
#endif

	typedef enum syscall_kind {
		SYSCALL_START_ALIVE,

		SYSCALL_TERM_WRITE,
		SYSCALL_TERM_CLEAR,
		SYSCALL_TERM_SET_TEXT_COLOR,
		SYSCALL_TERM_SET_BACKGROUND_COLOR,
		SYSCALL_TERM_GET_TEXT_COLOR,
		SYSCALL_TERM_GET_BACKGROUND_COLOR,
		SYSCALL_TERM_GET_CURSOR_X,
		SYSCALL_TERM_GET_CURSOR_Y,
		SYSCALL_TERM_SET_CURSOR_POS,
		SYSCALL_TERM_SCROLL,
		SYSCALL_TERM_WIDTH,
		SYSCALL_TERM_HEIGHT,
		SYSCALL_TERM_CLEAR_LINE,

		SYSCALL_HEAP_ALLOCATE,
		SYSCALL_HEAP_REALLOCATE,
		SYSCALL_HEAP_FREE,
		SYSCALL_HEAP_GET_PTR_SIZE,
		SYSCALL_HEAP_GET_USED_SIZE,

		SYSCALL_END
	} syscall_kind_t;

	typedef void(*syscall_handler_t)(registers_t* regs);
	
	void syscall_init(void);
	void syscall_register(syscall_kind_t kind, syscall_handler_t handler);
	
#if __cplusplus
}
#endif

#endif