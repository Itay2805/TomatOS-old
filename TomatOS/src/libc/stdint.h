#ifndef LIBC_STDINT_H
#define LIBC_STDINT_H

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

#define INT64_MAX   ((int64_t)(9223372036854775807))
#define INT64_MIN   (-(int64_t)(9223372036854775807) - 1)
#define UINT64_MAX  ((uint64_t)(18446744073709551615))

#define INT32_MAX   ((int32_t)(2147483647))
#define INT32_MIN   (-(int32_t)(2147483647) - 1)
#define UINT32_MAX  ((uint32_t)(4294967295))

#define INT16_MAX   ((int16_t)(32767))
#define INT16_MIN   (-(int16_t)(32767) - 1)
#define UINT16_MAX  ((uint16_t)(65535))

#define INT8_MAX    ((int8_t)(127))
#define INT8_MIN    (-(int8_t)(127) - 1)
#define UINT8_MAX   ((uint8_t)(255))

#endif