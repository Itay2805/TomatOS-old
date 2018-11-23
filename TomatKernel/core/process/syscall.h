#ifndef KERNEL_CORE_PROCESS_SYSCALL_H
#define KERNEL_CORE_PROCESS_SYSCALL_H

#include <kernel.h>
#include <tomato/syscalls.h>

#define CHECK_POINTER(ptr) ((ptr) > &tomatkernel_end + 1024 * 1024 * 50)

#if __cplusplus
extern "C" {
#endif

	typedef tomato_syscall_kind_t syscall_kind_t;

	typedef struct process_stats {
		uint32_t uid;
		uint32_t user;
	} process_stats_t;

	typedef void(*syscall_handler_t)(registers_t* regs);
	
	void syscall_init(void);
	void syscall_register(syscall_kind_t kind, syscall_handler_t handler);
	
#if __cplusplus
}
#endif

#endif