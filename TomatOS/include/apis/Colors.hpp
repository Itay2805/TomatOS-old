#ifndef TOMATO_API_COLORS_HPP
#define TOMATO_API_COLORS_HPP

#include <stdint.h>

namespace Tomato {

	namespace Colors {

		enum Color : uint8_t {
			BLACK			= 0x0,
			BLUE			= 0x1,
			GREEN			= 0x2,
			CYAN			= 0x3,
			RED				= 0x4,
			MAGENTA			= 0x5,
			BROWN			= 0x6,
			GRAY			= 0x7,
			DARK_GRAY		= 0x8,
			LIGHT_BLUE		= 0x9,
			LIGHT_GREEN		= 0xa,
			LIGHT_CYAN		= 0xb,
			LIGHT_RED		= 0xc,
			LIGHT_MAGENTA	= 0xd,
			YELLOW			= 0xe,
			WHITE			= 0xf
		};

	}

}

#endif