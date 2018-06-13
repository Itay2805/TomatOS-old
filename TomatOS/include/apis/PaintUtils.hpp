#ifndef TOMATO_PAINT_UTILS_HPP
#define TOMATO_PAINT_UTILS_HPP

#include <stdint.h>
#include <apis/Colors.hpp>
#include <cpp.hpp>

namespace Tomato {

	namespace PaintUtils {

		class Image {
		private:
			uint16_t width, height;
			Colors::Color* colors;

			bool autofree;

		public:
			Image(uint16_t width, uint16_t height, Colors::Color* data = nullptr);
			Image(const char* path);
			
			inline void SetPixel(Colors::Color color, uint16_t x, uint16_t y) { colors[x + y * width] = color; }
			Colors::Color GetPixel(uint16_t x, uint16_t y) const { return colors[x + y * width]; }

			inline uint16_t GetWidth() const { return width; }
			inline uint16_t GetHeight() const { return height; }

			void Save(const char* path);
			void Draw(uint16_t x, uint16_t y);

			void Resize(uint16_t width, uint16_t height);

			void Free();

		};



	}

}

#endif