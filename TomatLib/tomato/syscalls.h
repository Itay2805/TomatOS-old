#ifndef TOMATOLIB_H
#define TOMATOLIB_H

typedef enum tomato_syscall_kind {

	SYSCALL_START,

	SYSCALL_TERM_WRITE,
	SYSCALL_TERM_CLEAR,
	SYSCALL_TERM_SET_TEXT_COLOR,
	SYSCALL_TERM_SET_BACKGROUND_COLOR,
	SYSCALL_TERM_GET_TEXT_COLOR,
	SYSCALL_TERM_GET_BACKGROUND_COLOR,
	SYSCALL_TERM_SET_CURSOR_POS,
	SYSCALL_TERM_GET_CURSOR_X,
	SYSCALL_TERM_GET_CURSOR_Y,
	SYSCALL_TERM_SCROLL,
	SYSCALL_TERM_WIDTH,
	SYSCALL_TERM_HEIGHT,
	SYSCALL_TERM_CLEAR_LINE,
	SYSCALL_TERM_REDIRECT,
	SYSCALL_TERM_CURRENT,
	SYSCALL_TERM_NATIVE,

	SYSCALL_WINDOW_CREATE,
	SYSCALL_WINDOW_DELETE,
	SYSCALL_WINDOW_WRITE,
	SYSCALL_WINDOW_CLEAR,
	SYSCALL_WINDOW_SET_TEXT_COLOR,
	SYSCALL_WINDOW_SET_BACKGROUND_COLOR,
	SYSCALL_WINDOW_GET_TEXT_COLOR,
	SYSCALL_WINDOW_GET_BACKGROUND_COLOR,
	SYSCALL_WINDOW_GET_CURSOR_X,
	SYSCALL_WINDOW_GET_CURSOR_Y,
	SYSCALL_WINDOW_SET_CURSOR_POS,
	SYSCALL_WINDOW_SCROLL,
	SYSCALL_WINDOW_WIDTH,
	SYSCALL_WINDOW_HEIGHT,
	SYSCALL_WINDOW_CLEAR_LINE,
	SYSCALL_WINDOW_SET_VISIBLE,
	SYSCALL_WINDOW_REDRAW,
	SYSCALL_WINDOW_GET_X,
	SYSCALL_WINDOW_GET_Y,
	SYSCALL_WINDOW_RESTORE_CURSOR,
	SYSCALL_WINDOW_REPOSITION,

	SYSCALL_HEAP_ALLOCATE,
	SYSCALL_HEAP_REALLOCATE,
	SYSCALL_HEAP_FREE,
	SYSCALL_HEAP_GET_PTR_SIZE,
	SYSCALL_HEAP_GET_USED_SIZE,

	SYSCALL_OS_PULL_EVENT,
	SYSCALL_OS_QUEUE_EVENT,
	SYSCALL_OS_START_TIMER,
	SYSCALL_OS_CANCEL_TIMER,
	SYSCALL_OS_PROCESS_KILL,

	SYSCALL_END
} tomato_syscall_kind_t;

#endif