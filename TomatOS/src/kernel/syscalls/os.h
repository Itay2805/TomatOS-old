#ifndef SYSCALL_OS_H
#define SYSCALL_OS_H

#include <stdint.h>
#include <tomato.h>

void syscall_os_init(void);
void os_kqueue_event(event_t event);

#endif
