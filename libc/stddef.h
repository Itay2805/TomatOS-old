#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#define NULL (0)
#define UNUSED(x) (void)(x)

#define offsetof(type, element) ((unsigned long)&(((type *)0)->element))

#define alloca(p, N) \
    do { \
        __asm__ __volatile__( \
        "sub %1, %%esp \n" \
        "mov %%esp, %0  \n" \
         : "=m"(p) \
         : "i"(N) \
         : "esp"); \
    } while(0)

#if __cplusplus
extern "C" {
#endif

#if __cplusplus
}
#endif

#endif