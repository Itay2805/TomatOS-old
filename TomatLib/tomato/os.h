#ifndef TOMATO_OS_H
#define TOMATO_OS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum tomato_event_kind {
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

	static inline void tomato_os_kill(uint32_t id) {
	}

	static inline void tomato_os_pull_event(tomato_event_t* event) {
	}

	static inline void tomato_os_queue_event(tomato_event_t* event, uint32_t uid) {
	}

	static inline void tomato_os_run(bool handle_events) {
		
	}

#if __cplusplus
}
#endif

#endif