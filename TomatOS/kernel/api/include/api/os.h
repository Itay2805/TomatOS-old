#ifndef API_OS_H
#define API_OS_H

#include <stdint.h>

typedef struct {
	uint32_t id;
} timer_t;

const char* os_version(void);
const char* os_get_computer_label(void);
timer_t os_start_timer(uint32_t timeout);
void os_cancel_timer(timer_t timer);
void os_sleep(uint32_t timeout);
void os_shutdown(void);
void os_reboot(void);

#endif