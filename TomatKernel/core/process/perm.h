#ifndef KERNEL_CORE_PERM_H
#define KERNEL_CORE_PERM_H

#include <stdint.h>

// This makes
#define USER_GUEST ((uint8_t)0)
// GUEST user can only read, write and execute files that are set to GUEST user, nothing else

#define USER_ROOT ((uint8_t)0xFE)
// ROOT can read, write and execute any files

#define USER_ALIVE ((uint8_t)0xFF)
// ALIVE can user processes runs under the kernel segments instead of the default user segments

#ifdef __cplusplus
extern "C" {
#endif
	
	// TODO: Can only implement this after files are working...
	// void perm_init();

#ifdef __cplusplus
}
#endif

#endif