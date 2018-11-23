#ifndef TOMATO_HEAP_HPP
#define TOMATO_HEAP_HPP

#if !defined(__cplusplus)
	#error("Tomato::Heap is a C++ library and can only be included inside a C++ file")
#endif

#include <stdint.h>
#include <tomato/heap.h>

namespace Tomato {
	namespace Heap {

		inline uintptr_t Allocate(size_t size) {
			return tomato_heap_allocate(size);
		}

		inline void Free(uintptr_t ptr) {
			tomato_heap_free(ptr);
		}

		inline uintptr_t Reallocate(uintptr_t ptr, size_t size) {
			return tomato_heap_reallocate(ptr, size);
		}

		inline size_t GetPtrSize(uintptr_t ptr) {
			return tomato_heap_get_ptr_size(ptr);
		}

		inline size_t GetUsedSize() {
			return tomato_heap_get_used_size();
		}

	}
}

inline void *operator new(size_t size) { return Tomato::Heap::Allocate(size); }
inline void *operator new[](size_t size) { return Tomato::Heap::Allocate(size); }
inline void operator delete(void *p) { Tomato::Heap::Free((uintptr_t)p); }
inline void operator delete[](void *p) { Tomato::Heap::Free((uintptr_t)p); }

inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() {};


#endif