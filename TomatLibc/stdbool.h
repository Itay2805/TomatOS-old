#ifndef LIBC_BOOL_H
#define LIBC_BOOL_H

#if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
    #define __bool_true_false_are_defined 1

    typedef char bool;
    #define true ((bool)1)
    #define false ((bool)0)
#endif

#endif