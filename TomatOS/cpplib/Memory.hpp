#ifndef CPPLIB_MEM_HPP
#define CPPLIB_MEM_HPP

namespace cpplib {

	template<class Data>
	void VecAlloc(Data*& pointer, size_t size) {
		if (size == 0) {
			pointer = nullptr;
		}
		else {
			pointer = new Data[size];
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
	void VecReset(Data*& pointer, size_t newsize) {
		VecFree(pointer);
		VecAlloc(pointer, newsize);
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

}

#endif