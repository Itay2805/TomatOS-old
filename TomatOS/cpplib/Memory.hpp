#ifndef CPPLIB_MEM_HPP
#define CPPLIB_MEM_HPP

#include <stddef.h>

#include <Status.hpp>

namespace cpplib {

	template<class Data>
	Status VecAlloc(Data*& pointer, size_t size) {
		if (size == 0) {
			pointer = nullptr;
			return OK;
		}
		else {
			DOPr(pointer = new Data[size]);
			return OK;
		}
	}

	template<class Data>
	void VecFree(Data*& pointer) {
		if (pointer != nullptr) {
			delete[] pointer;
			pointer = nullptr;
		}
	}

	template<class Data>
	Status VecReset(Data*& pointer, size_t newsize) {
		VecFree(pointer);
		return VecAlloc(pointer, newsize);
	}

	inline size_t NewSize(size_t size, size_t minNewSize, int delta = -1) {
		size_t newsize = size;
		if (delta == -1) {
			for (; newsize < minNewSize; newsize = newsize * 2 + 1);
		}
		else {
			for (; newsize < minNewSize; newsize = newsize + delta);
		}
		return newsize;
	}


	template<class Data>
	struct Pointer {
		Data* ptr;
		size_t size;

		Status Alloc(size_t size) {
			return VecAlloc(ptr, this->size = size);
		}

		void Free() {
			VecFree(ptr);
			size = 0;
		}

		Status Reset(size_t newsize) {
			if (newsize != size) {
				return VecReset(ptr, size = newsize);
			}
			return OK;
		}

		Status Realloc(size_t newsize, Data*& oldp) {
			if (newsize != size) {
				oldp = ptr;
				return VecAlloc(ptr, size = newsize);
			}
			else {
				oldp = NULL;
				return OK;
			}
		}

		Pointer(size_t size = 0) {
			Alloc(size);
		}

		operator Data*() { return ptr; }
		operator const Data*() const { return ptr; }

		Data& At(size_t index) { return ptr[index]; }
		const Data& At(size_t index) const { return ptr[index]; }
		Data& operator[](size_t index) { return At(index); }
		const Data& operator[](size_t index) const { return At(index); }

		Status Copy(size_t at, const Data* from, size_t howmany) {
			if (at + howmany > size) {
				return ERROR_RANGE;
			}
			memcpy(ptr + at, from, howmany * sizeof(Data));
			return OK;
		}

		void Share(const Pointer<Data>& from) {
			ptr = from.ptr;
			size = from.size;
		}

		void Steal(Pointer<Data>& from) {
			Share(from);
			from.ptr = NULL;
			from.size = 0;
		}

		void operator=(const Pointer<Data>& from) {
			Share(from);
		}

	};

	template<class Data>
	void Duplicate(Pointer<Data>& to, const Pointer<Data>& from) {
		to.Reset(from.size);
		for (size_t i = 0; i < from.size; ++i) {
			Duplicate(to.ptr[i], from.ptr[i]);
		}
	}

	template<class Data>
	void Free(Pointer<Data>& p) {
		for (size_t i = 0; i < p.size; ++i) {
			Free(p.ptr[i]);
		}
		p.Free();
	}

}

#endif