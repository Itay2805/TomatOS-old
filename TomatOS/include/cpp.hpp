#ifndef CPP_HPP
#define CPP_HPP

#ifdef __cplusplus

#include <stddef.h>

inline void *operator new(size_t size)
{
	return malloc(size);
}

inline void *operator new[](size_t size)
{
	return malloc(size);
}

inline void operator delete(void *p)
{
	free(p);
}

inline void operator delete[](void *p)
{
	free(p);
}

inline void *operator new(size_t, void *p) { return p; }
inline void *operator new[](size_t, void *p) { return p; }
inline void  operator delete  (void *, void *) { };
inline void  operator delete[](void *, void *) {};

#endif

#endif