#ifndef LIBC_STDBOOL_H
#define LIBC_STDBOOL_H

// we always define the bool_t type
#include <stdint.h>
typedef uint8_t bool_t;

// only enable this if we are not in C++
#ifndef __cplusplus

#define bool bool_t;
#define true ((bool_t)1)
#define false ((bool_t)0)

#endif

#endif