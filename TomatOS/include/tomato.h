#ifndef TOMATO_H
#define TOMATO_H

// this will contain all the system calls
// for C++ headers look at the apis include dir

#include <syscall.h>

/////////////////////////////////////////////////////
//// term API
/////////////////////////////////////////////////////

inline static void tomato_term_write(const char* text) { 
	tomato_syscall_1p(TOMATO_SYSCALL_TERM_WRITE, (uint32_t)text);
}

#endif