#ifndef TOMATO_COLOR_HPP
#define TOMATO_COLOR_HPP

#if !defined(__cplusplus)
	#error("Tomato::Color is a C++ library and can only be included inside a C++ file")
#endif

#include <stdint.h>

namespace Tomato {

	enum class Color : uint8_t {
		BLACK = 0x0,
		BLUE = 0x1,
		GREEN = 0x2,
		CYAN = 0x3,
		RED = 0x4,
		MAGENTA = 0x5,
		BROWN = 0x6,
		GRAY = 0x7,
		DARK_GRAY = 0x8,
		LIGHT_BLUE = 0x9,
		LIGHT_GREEN = 0xa,
		LIGHT_CYAN = 0xb,
		LIGHT_RED = 0xc,
		LIGHT_MAGENTA = 0xd,
		YELLOW = 0xe,
		WHITE = 0xf
	};

}

#endif