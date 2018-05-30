#ifndef LIBC_LIMITS_H
#define LIBC_LIMITS_H

#define INT64_MAX   ((long long)(9223372036854775807))
#define INT64_MIN   (-(long long)(9223372036854775807) - 1)
#define UINT64_MAX  ((unsigned long long)(18446744073709551615))

#define INT32_MAX   ((long)(2147483647))
#define INT32_MIN   (-(long)(2147483647) - 1)
#define UINT32_MAX  ((unsigned long)(4294967295))

#define INT16_MAX   ((short)(32767))
#define INT16_MIN   (-(short)(32767) - 1)
#define UINT16_MAX  ((unsigned short)(65535))

#define INT8_MAX    ((char)(127))
#define INT8_MIN    (-(char)(127) - 1)
#define UINT8_MAX   ((unsigned char)(255))

#endif