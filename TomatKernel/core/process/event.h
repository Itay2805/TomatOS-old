#ifndef KERNEL_CORE_PROCESS_EVENT_H
#define KERNEL_CORE_PROCESS_EVENT_H

#include <stdint.h>

typedef enum event_type {
	EVENT_ALL,
	EVENT_KEY,
} event_type_t;

typedef struct event {
	event_type_t type;
	uint32_t data[4];

} event_t;

#ifdef __cplusplus
extern "C" {
#endif

	void event_init(void);
	void event_pull_blocking(event_t* event, event_type_t filter);

#ifdef __cplusplus
}
#endif

#endif