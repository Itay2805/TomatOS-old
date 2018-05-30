#ifndef CPPLIB_TYPEDEFS_HPP
#define CPPLIB_TYPEDEFS_HPP

#include <string.h>

namespace cpplib {

	typedef const char* CStr;

#define DEFINE_INLINE_RELATIVE_OPERATOR(T, op) inline bool operator op(const T& a, const T& b) { return ::cpplib::Compare(a, b) op 0; }

#define DEFINE_INLINE_RELATIVE_OPERATORS(T)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,>)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,<)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,==)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,<=)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,>=)\
DEFINE_INLINE_RELATIVE_OPERATOR(T,!=)

#define DEFINE_USEFUL_FUNCTIONS(T)\
	inline short Compare (T a, T b) {return  a>b? 1: a==b? 0: -1;}\
	inline bool  Identical (T a, T b) {return  a==b;}\
	inline void Duplicate (T& to, const T& from) { to=from; }\
	inline void Free (T&) { } \

	DEFINE_USEFUL_FUNCTIONS(uint8_t);
	DEFINE_USEFUL_FUNCTIONS(uint16_t);
	DEFINE_USEFUL_FUNCTIONS(uint32_t);
	DEFINE_USEFUL_FUNCTIONS(uint64_t);

	DEFINE_USEFUL_FUNCTIONS(int8_t);
	DEFINE_USEFUL_FUNCTIONS(int16_t);
	DEFINE_USEFUL_FUNCTIONS(int32_t);
	DEFINE_USEFUL_FUNCTIONS(int64_t);

	DEFINE_USEFUL_FUNCTIONS(float);
	DEFINE_USEFUL_FUNCTIONS(double);

	inline uint64_t HashValue(uint8_t a) { return (uint64_t)a; }
	inline uint64_t HashValue(uint16_t a) { return (uint64_t)a; }
	inline uint64_t HashValue(uint32_t a) { return (uint64_t)a; }
	inline uint64_t HashValue(uint64_t a) { return (uint64_t)a; }

	inline uint64_t HashValue(int8_t a) { return (uint64_t)a; }
	inline uint64_t HashValue(int16_t a) { return (uint64_t)a; }
	inline uint64_t HashValue(int32_t a) { return (uint64_t)a; }
	inline uint64_t HashValue(int64_t a) { return (uint64_t)a; }

	inline uint64_t HashValue(float a) { return (uint64_t)(*(uint32_t*)&a); }
	inline uint64_t HashValue(double a) { return *(uint64_t*)&a; }

	inline short Compare(CStr a, CStr b) { return strcmp(a, b); }

	inline uint64_t HashValue(CStr a) {
		uint64_t sum = 0;
		for (size_t i = 0; i < strlen(a); ++i) sum += a[i];
		return sum;
	}

	inline void Duplicate(CStr& to, const CStr& from) { to = from; }
	inline void Free(CStr&) {}

	template<class Data>
	Data Duplicate(const Data& source) {
		Data dup;
		Duplicate(dup, source);
		return dup;
	}

}



#endif