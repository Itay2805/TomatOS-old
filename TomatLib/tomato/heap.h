#ifndef TOMATO_HEAP_H
#define TOMATO_HEAP_H

#include <stdint.h>

#include <tomato/syscalls.h>

#if __cplusplus
extern "C" {
#endif

	static inline uintptr_t tomato_heap_allocate(size_t size) {
		uintptr_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_HEAP_ALLOCATE), "b"(size));
		return ret;
	}

	static inline uintptr_t tomato_heap_reallocate(uintptr_t ptr, size_t size) {
		uintptr_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_HEAP_REALLOCATE), "b"(ptr), "c"(size));
		return ret;
	}

	static inline void tomato_heap_free(uintptr_t ptr) {
		asm("int $0x80" : : "a"(SYSCALL_HEAP_FREE), "b"(ptr));
	}

	static inline size_t tomato_heap_get_ptr_size(uintptr_t ptr) {
		size_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_HEAP_GET_PTR_SIZE), "b"(ptr));
		return ret;
	}

	static inline size_t tomato_heap_get_used_size() {
		size_t ret;
		asm("int $0x80" : "=a"(ret) : "a"(SYSCALL_HEAP_GET_USED_SIZE));
		return ret;
	}

#if __cplusplus
}
#endif

#endif