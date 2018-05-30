#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#define NULL (0)
#define UNUSED(x) (void)(x)
#define STR(x) #x

#define offsetof(type, element) ((unsigned long)&(((type *)0)->element))

#endif
