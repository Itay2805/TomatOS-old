#ifndef LIBC_STDINT_H
#define LIBC_STDINT_H

#include <limits.h>

typedef char                int8_t;
typedef short               int16_t;
typedef long                int32_t;
typedef long long           int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long       uint32_t;
typedef unsigned long long  uint64_t;

typedef uint8_t*            uintptr_t;
typedef uint32_t			size_t;

#endif