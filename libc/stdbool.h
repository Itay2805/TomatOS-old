#ifndef LIBC_STDBOOL_H
#define LIBC_STDBOOL_H

#include <stdint.h>

#ifndef __cplusplus
	#define bool    uint8_t
	#define false   ((bool)0)
	#define true    ((bool)1)
#endif

#endif