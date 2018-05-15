#ifndef LIBC_STDBOOL_H
#define LIBC_STDBOOL_H

// we always define the bool_t type
#include <stdint.h>

// only enable this if we are not in C++
#ifndef __cplusplus

typedef uint8_t bool;
#define true ((bool)1)
#define false ((bool)0)

#endif

#endif