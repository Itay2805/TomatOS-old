#ifndef LIBC_LIMITS_H
#define LIBC_LIMITS_H

#define INT64_MAX   ((signed long long)(9223372036854775807))
#define INT64_MIN   (-(signed long long)(9223372036854775807) - 1)
#define UINT64_MAX  ((unsigned long long)(18446744073709551615))

#define INT32_MAX   ((signed long)(2147483647))
#define INT32_MIN   (-(signed long)(2147483647) - 1)
#define UINT32_MAX  ((unsigned long)(4294967295))

#define INT16_MAX   ((signed short)(32767))
#define INT16_MIN   (-(signed short)(32767) - 1)
#define UINT16_MAX  ((unsigned short)(65535))

#define INT8_MAX    ((signed char)(127))
#define INT8_MIN    (-(signed char)(127) - 1)
#define UINT8_MAX   ((unsigned char)(255))

#define SIZE_MAX	(size_t)UINT32_MAX
#define UINTPTR_MAX (uintptr_t)UINT32_MAX

#endif