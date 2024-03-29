#ifndef KERNEL_CORE_TIMER_H
#define KERNEL_CORE_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	void timer_init(void);

	uint32_t timer_start(uint32_t uid, uint32_t millis);

	// if the ID is 0 it will remove all the timers for process
	void timer_cancel(uint32_t uid, uint32_t timer);

#ifdef __cplusplus
}
#endif

#endif