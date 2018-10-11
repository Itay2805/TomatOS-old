#ifndef LIBC_STDINT_H
#define LIBC_STDINT_H

#include <limits.h>

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed long         int32_t;
typedef signed long long    int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long       uint32_t;
typedef unsigned long long  uint64_t;

typedef int32_t             intptr_t;
typedef uint32_t            uintptr_t;

#define INT8_MIN ((int8_t)SCHAR_MIN)
#define INT16_MIN ((int64_t)SHRT_MIN)
#define INT32_MIN ((int64_t)LONG_MIN)
#define INT64_MIN ((int64_t)LLONG_MIN)

#define INT8_MAX ((int8_t)SCHAR_MAX)
#define INT16_MAX ((int64_t)SHRT_MAX)
#define INT32_MAX ((int64_t)LONG_MAX)
#define INT64_MAX ((int64_t)LLONG_MAX)

#define UINT8_MAX ((uint8_t)UCHAR_MAX)
#define UINT16_MAX ((uint64_t)USHRT_MAX)
#define UINT32_MAX ((uint64_t)ULONG_MAX)
#define UINT64_MAX ((uint64_t)ULLONG_MAX)

#define INTPTR_MIN ((int64_t)INT32_MIN)
#define INTPTR_MAX ((int64_t)INT32_MAX)
#define UINTPTR_MAX ((int64_t)UINT32_MAX)

#endif