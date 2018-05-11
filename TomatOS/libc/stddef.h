#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#define NULL (0)
#define UNUSED(x) (void)(x)
#define STR(x) #x

#define offsetof(type, element) ((unsigned long)&(((TYPE *)0)->ELEMENT))

#endif
