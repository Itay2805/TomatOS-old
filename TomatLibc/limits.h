#ifndef LIBC_LIMITS_H
#define LIBC_LIMITS_H

#define CHAR_BIT    8
#define SCHAR_MIN   ((signed char)-128)
#define SCHAR_MAX   ((signed char)127)
#define UCHAR_MAX   ((unsigned char)255)
#define CHAR_MIN    0
#define CHAR_MAX    UCHAR_MAX

#define SHRT_MIN    ((signed short)-32768)
#define SHRT_MAX    ((signed short)32767)
#define USHRT_MAX    ((unsigned short)65535)

// note, these depend on compile, but will generally work fine
#define INT_MIN    ((signed int)-2147483648)
#define INT_MAX    ((signed int)2147483647)
#define UINT_MAX    ((unsigned int)4294967295)

#define LONG_MIN    ((signed long)-2147483648)
#define LONG_MAX    ((signed long)2147483647)
#define ULONG_MAX    ((unsigned long)4294967295)

#define LLONG_MIN    ((signed long long)-9223372036854775807)
#define LLONG_MAX    ((signed long long)9223372036854775807)
#define ULLONG_MAX    ((unsigned long long)18446744073709551615)

#endif