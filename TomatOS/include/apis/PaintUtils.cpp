#include "PaintUtils.hpp"

#include <string.h>

#include <apis/FS.hpp>

namespace Tomato {

	namespace PaintUtils {

		Image::Image(uint16_t width, uint16_t height, Colors::Color* data) 
			: width(width)
			, height(height)
		{
			colors = new Colors::Color[width * height];
			if (data != nullptr) {
				memcpy(colors, data, width * height * sizeof(Colors::Color));
			}
		}

		Image::Image(const char* path) {
			if (FS::Exists(path)) {
				FS::File file = FS::File(path, FS::File::READ);
				if (!file.IsDir()) {
					width = file.Read<uint16_t>();
					height = file.Read<uint16_t>();
					colors = new Colors::Color[width * height];
					for (int i = 0; i < width * height; i++) {
						colors[i] = file.Read<Colors::Color>();
					}
				}
			}
		}

		void Image::Save(const char* path) {
			FS::File file = FS::File(path, FS::File::WRITE);
			file.Write<uint16_t>(width);
			file.Write<uint16_t>(height);
			for (int i = 0; i < width * height; i++) {
				file.Write<Colors::Color>(colors[i]);
			}
		}

		void Image::Resize(uint16_t width, uint16_t height) {
			Colors::Color* temp = new Colors::Color[width * height];
			int w = width > this->width ? this->width : width;
			int h = height > this->height ? this->height : height;
			for (uint16_t y = 0; y < h; y++) {
				for (uint16_t x = 0; x < w; x++) {
					temp[x + y * width] = colors[x + y * this->width];
				}
			}
			delete[] colors;
			colors = temp;
			this->width = width;
			this->height = height;
		}

		void Image::Draw(uint16_t _x, uint16_t _y) {
			if (colors == nullptr) return;
			int w = width + _x > Term::GetWidth() ? Term::GetWidth() - _x : width;
			int h = height + _y > Term::GetHeight() ? Term::GetHeight() - _y : height;
			for (uint16_t y = 0; y < h; y++) {
				Term::SetCursorPos(_x, y + _y);
				for (uint16_t x = 0; x < w; x++) {
					Term::SetBackgroundColor(colors[x + y * width]);
					Term::Write(" ");
				}
			}
		}

		void Image::Free() {
			if (colors != nullptr) {
				delete[] colors;
				colors = nullptr;
			}
		}

	}

}