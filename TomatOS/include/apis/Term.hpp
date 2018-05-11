#ifndef TOMATO_API_TERM_H
#define TOMATO_API_TERM_H

#include <tomato.h>
#include <stdint.h>

namespace Tomato {

	class Term {
	public:
		virtual void WriteInternal(const char* text) = 0;

	private:
		static Term* current;

	public:

		static inline void Write(const char* text) { current->WriteInternal(text); }

		static inline Term* Current() { return current; }
		static Term* Native();
	};

}

#endif