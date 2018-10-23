#ifndef LIBC_SETJMP_H
#define LIBC_SETJMP_H

#ifdef __GNUC__

    typedef struct jmp_buf {
        int val;
        void* _ptr[5];
    } jmp_buf[1];

    static inline void setjmp(jmp_buf buf) {
        buf->val = 0;
        __builtin_setjmp(buf);
        buf->val;
    }

    static inline void longjmp(jmp_buf buf, int val) {
        buf->val = val == 0 ? 1 : val;
        __builtin_longjmp(buf, val);
    }

#else
    #error "setjmp.h only works on GCC"
#endif

#endif