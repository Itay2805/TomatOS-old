#ifndef TOMATO_OS_H
#define TOMATO_OS_H

#include <stdint.h>
#include <stdbool.h>

#include <tomato/syscalls.h>

typedef enum tomato_event_kind {
	TOMATO_EVENT_ANY,
	TOMATO_EVENT_TERMINATE,
	TOMATO_EVENT_TIMER,
} tomato_event_kind_t;

typedef struct tomato_event {
	tomato_event_kind_t kind;
	uint32_t data[4];
} tomato_event_t;

#if __cplusplus
extern "C" {
#endif

	static inline uint32_t tomato_os_start_timer(uint32_t millis) {
		uint32_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_OS_START_TIMER), "b"(millis));
		return ret;
	}

	static inline void tomato_os_cancel_timer(uint32_t tid) {
		asm("int $0x80" : : "a"(SYSCALL_OS_CANCEL_TIMER), "b"(tid));
	}

	static inline void tomato_os_kill(uint32_t uid) {
		UNUSED(uid);

		// not implemented
	}

	static inline void tomato_os_pull_event(tomato_event_t* event) {
		tomato_event_t* ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_OS_PULL_EVENT));

		// we don't have to worry over memory overrite since we are in user space
		// also the reason we make a copy is because the event pointer is global for the application
		// so for every new event from pull has the same pointer (at least for now)
		*event = *ret;
	}

	static inline void tomato_os_queue_event(tomato_event_t* event, uint32_t uid) {
		asm("int $0x80" : : "a"(SYSCALL_OS_QUEUE_EVENT), "b"(event), "c"(uid));
	}

	static inline void tomato_os_run() {
		// TODO
	}

#if __cplusplus
}
#endif

#endif