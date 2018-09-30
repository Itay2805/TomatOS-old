#ifndef KERNEL_CORE_PERM_H
#define KERNEL_CORE_PERM_H

#include <stdint.h>
#include <stdbool.h>

#define USER_ROOT ((uint8_t)0xFE)
// ROOT can read, write and execute any files

#define USER_ALIVE ((uint8_t)0xFF)
// ALIVE can user processes runs under the kernel segments instead of the default user segments

#ifdef __cplusplus
extern "C" {
#endif
	
	// TODO: Can only implement this after files are working...
	// void perm_init();

	typedef uint8_t user_t;

	static inline bool has_permission(user_t required, user_t has) {
		if (required == 0) {
			return true;
		}
		return has > (required - 1);
	}

#ifdef __cplusplus
}
#endif

#endif