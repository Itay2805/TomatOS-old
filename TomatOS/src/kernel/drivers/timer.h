#ifndef KERNEL_DRIVER_TIMER_H
#define KERNEL_DRIVER_TIMER_H

#include <stdint.h>

uint32_t driver_start_timer(uint32_t timeout);
void driver_cancel_timer(uint32_t id);

void driver_timer_init(void);

#endif