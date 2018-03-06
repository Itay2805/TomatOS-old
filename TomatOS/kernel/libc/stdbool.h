#ifndef LIBC_STDBOOL_H
#define LIBC_STDBOOL_H

#include <stdint.h>

typedef uint8_t bool_t;

#define bool bool_t
#define true ((bool_t)1)
#define false ((bool_t)0)

#endif