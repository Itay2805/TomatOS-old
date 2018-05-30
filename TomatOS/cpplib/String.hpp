#ifndef CPPLIB_STRING_HPP
#define CPPLIB_STRING_HPP

#include <stdint.h>
#include <string.h>

#include <Memory.hpp>
#include <Typedefs.hpp>
#include <Tmplate.hpp>

namespace cpplib {
	/*
	class String {
	private:
		char* str;
		size_t size;
		size_t len;

	private:
		void Alloc(size_t size) {
			this->len = 0;
			VecAlloc(str, this->size = size);
		}

		void Free() {
			VecFree(str);
			Alloc(0);
		}

		void Reset(size_t size) {
			this->len = 0;
			if (this->size != size) {
				VecReset(str, this->size = size);
			}
		}

		void _Steal(String& str) {
			this->str = str.str;
			this->size = str.size;
			this->len = str.len;
			str.Alloc(0);
		}

		void _Set(const char* str) {
			Alloc(strlen(str) + 1);
			this->len = size - 1;
			strcpy(this->str, str);
		}

		void _Set(const char* str, size_t size) {
			Alloc(size);
			this->len = strlen(str);
			strcpy(this->str, str);
		}

	public:

		String() { Alloc(0); }
		String(size_t size) { Alloc(size); }
		String(const char* str) { _Set(str); }
		String(const char* str, size_t size) { _Set(str, size); }
		String(const String& str) {
			*this = str.Clone();
		}
		~String() { Free(); }

		void Steal(String& str) {
			Free();
			_Steal(str);
		}

		void Set(CStr str) {
			Free();
			_Set(str);
		}

		void Set(CStr str, size_t size) {
			Free();
			_Set(str, size);
		}

		void operator=(const String& str) {
			Free();
			*this = str.Clone();
		}

		bool IsValid() { return str != nullptr; }
		bool IsEmpty() { return len == 0; }
		
		char& At(size_t i) { return str[i]; }
		char At(size_t i) const { return str[i]; }
		char& AtEnd(size_t i) { return str[len - 1 - i]; }
		char AtEnd(size_t i) const { return str[len - 1 - i]; }
		
		bool EndsWith(char c) const { return AtEnd(0) == c; }
		bool EndsWith(CStr str, size_t len) const { return strncmp(this->str + this->len - len, str, len) == 0; }
		bool EndsWith(CStr str) const { return EndsWith(str, strlen(str)); }
		bool EndsWith(const String& str) const { return EndsWith(str.str, str.len); }
		
		bool StartsWith(char c) const { return str[0] == c; }
		bool StartsWith(CStr str, size_t len) const { return strncmp(this->str, str, len) == 0; }
		bool StartsWith(CStr str) const { return StartsWith(str, strlen(str)); }
		bool StartsWith(const String& str) const { return StartsWith(str.str, str.len); }

		char& operator[](size_t i) { return str[i]; }
		char operator[](size_t i) const { return str[i]; }

		char* FinalStr() {
			char* s = str;
			Alloc(0);
			return s;
		}

		char* Duplicate() const {
			char* s = new char[len + 1];
			strcpy(s, str);
			return s;
		}

		String Clone() const {
			return String(Duplicate(), size);
		}

		void End() {
			str[len] = '\0';
		}

		void Put(size_t at, CStr str, size_t f, size_t t) {
			if (t < f || at + t - f >= size) {
				// return error maybe
				return;
			}

			memcpy(this->str + at, str + f, t - f);
			len = Max(len, at + t - f);
			End();
		}

		void Put(size_t at, CStr str) {
			Put(at, str, 0, strlen(str));
		}

		void Put(size_t at, char c) {
			Put(at, &c, 0, 1);
		}

		void Put(size_t at, const String& str) {
			Put(at, str.str);
		}

		void Append(CStr str, size_t f, size_t t) {

		}

		const char* CStr() const {
			return str;
		}

		size_t Length() const {
			return len;
		}

		size_t Size() const {
			return size;
		}

	};

	inline short Compare(const String& a, const String& b) { return strcmp(a.CStr(), b.CStr()); }
	inline short Compare(const String& a, CStr b) { return strcmp(a.CStr(), b); }
	inline short Compare(CStr b, const String& a) { return strcmp(b, a.CStr()); }

	inline void Duplicate(String& to, const String& from) { to.Set(from.CStr(), from.Size()); }

	DEFINE_INLINE_RELATIVE_OPERATORS(String);
	*/
}

#endif