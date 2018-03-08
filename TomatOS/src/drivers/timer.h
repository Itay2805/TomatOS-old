#ifndef DRIVER_TIMER_H
#define DRIVER_TIMER_H

#include <stdint.h>

void timer_create(uint32_t id, uint32_t milis);
void timer_cancel(uint32_t id);

void kernel_timer_init();

#endif // !DRIVER_TIMER_H
