#ifndef TOMATO_SYSTEM_H
#define TOMATO_SYSTEM_H

// this will contain all the system calls
// for C++ headers look at the apis include dir

#include <syscall.h>

inline static void tomato_term_write(char* text) { tomato_syscall_1p(TOMATO_SYSCALL_TERM_WRITE, text); }

#endif