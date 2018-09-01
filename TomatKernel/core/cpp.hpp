#ifndef CPP_HPP
#define CPP_HPP

#include <stdlib.h>

inline void *operator new(size_t size) { return malloc(size); }
inline void *operator new[](size_t size) { return malloc(size); }
inline void operator delete(void *p) { free(p); }
inline void operator delete[](void *p) { free(p); }

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() {};

#endif