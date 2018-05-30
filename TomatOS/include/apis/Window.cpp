#include "Window.hpp"

namespace Tomato {


	Window::Window(ITerm* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible)
		: parent(parent)
		, x(x), y(y)
		, width(width), height(height)
		, cursorX(0), cursorY(0)
		, bgColor(0), fgColor(0xf)
		, visible(visible)
	{
		screenBuffer = new uint8_t[width * height * 2];
	}

	Window::~Window() {
		delete screenBuffer;
		screenBuffer = NULL;
	}

	void Window::Write(const char* text) {
		while (*text != 0) {
			char c = *text;
			if (c == '\n') {
				cursorY++;
				cursorX = 0;
			}
			else if (c == '\t') {
				screenBuffer[2 * (cursorX + cursorY * width) + 0] = 0;
				screenBuffer[2 * (cursorX + cursorY * width) + 1] = (uint8_t)(bgColor << 4 | fgColor);
				screenBuffer[2 * (cursorX + 1 + cursorY * width) + 0] = 0;
				screenBuffer[2 * (cursorX + 1 + cursorY * width) + 1] = (uint8_t)(bgColor << 4 | fgColor);
				cursorX += 2;
			}
			else {
				screenBuffer[2 * (cursorX + cursorY * width) + 0] = c;
				screenBuffer[2 * (cursorX + cursorY * width) + 1] = (uint8_t)(bgColor << 4 | fgColor);
				cursorX++;
			}
			text++;

			// go to next line if needed
			if (cursorX >= width) {
				cursorY++;
				cursorX = 0;
			}

			// scroll if needed
			if (cursorY >= height) {
				cursorY--;
				cursorX = 0;
				Scroll(1);
			}
		}
		if (visible) {
			Redraw();
		}
	}

	void Window::Clear() {
		for (int32_t i = 0; i < height * width * 2; i += 2) {
			screenBuffer[i + 0] = 0;
			screenBuffer[i + 1] = (uint8_t)(bgColor << 4 | fgColor);
		}
		if (visible) {
			Redraw();
		}
	}

	void Window::SetTextColor(uint8_t color) {
		fgColor = color;
	}

	void Window::SetBackgroundColor(uint8_t color) {
		bgColor = color;
	}

	void Window::SetCursorPos(uint16_t x, uint16_t y) {
		cursorX = x;
		cursorY = y;
	}

	uint16_t Window::GetCursorX() {
		return cursorX;
	}

	uint16_t Window::GetCursorY() {
		return cursorY;
	}

	uint16_t Window::GetWidth() {
		return width;
	}

	uint16_t Window::GetHeight() {
		return height;
	}

	uint8_t Window::GetTextColor() {
		return fgColor;
	}

	uint8_t Window::GetBackgroundColor() {
		return bgColor;
	}

	void Window::Scroll(uint16_t n) {
		int32_t length_copy = ((height - n) * width) * 2;
		int32_t length_remove = (n * width) * 2;
		for (int32_t i = 0; i < length_copy; i++) {
			screenBuffer[i] = screenBuffer[i + (n * width) * 2];
			screenBuffer[i + 1] = screenBuffer[1 + i + (n * width) * 2];
		}
		for (int32_t i = 0; i < length_remove; i++) {
			screenBuffer[i + length_copy] = 0;
			screenBuffer[i + 1 + length_copy] = (uint8_t)(bgColor << 4 | fgColor);
		}
		if (visible) {
			Redraw();
		}
	}

	void Window::ClearLine(uint16_t n) {
		int32_t offset = (n * width) * 2;
		for (int32_t i = offset; i < width * 2 + offset; i += 2) {
			screenBuffer[i + 0] = 0;
			screenBuffer[i + 1] = (uint8_t)(bgColor << 4 | fgColor);
		}
		if (visible) {
			Redraw();
		}
	}

	void Window::SetVisible(bool vis) {
		visible = vis;
		if (vis) {
			Redraw();
		}
	}

#define MIN(a, b) ((a) > (b)) ? (b) : (a)

	void Window::Redraw() {
		if (!visible) return;

		//uint16_t savedX = parent->get_cursor_x();
		//uint16_t savedY = parent->get_cursor_y();
		uint8_t savedTextColor = parent->GetTextColor();
		uint8_t savedBackgroundColor = parent->GetBackgroundColor();

		uint16_t width = MIN(this->width, parent->GetWidth() - this->width + this->x);
		uint16_t height = MIN(this->height, parent->GetHeight() - this->height + this->y);

		// we need something better for this :think:
		char buff[2];
		buff[1] = 0;
		for (uint16_t y = 0; y < height; y++) {

			// prepare the next row
			parent->SetCursorPos(this->x, (uint16_t)(this->y + y));
			for (uint16_t x = 0; x < width; x++) {
				char c = (char)screenBuffer[(x + y * this->width) * 2];
				if (c == 0) c = ' ';
				buff[0] = c;
				uint8_t color = screenBuffer[(x + y * this->width) * 2 + 1];
				uint8_t fg = color & 0xF;
				uint8_t bg = (color >> 4) & 0xF;

				parent->SetTextColor(fg);
				parent->SetBackgroundColor(bg);
				parent->Write(buff);
			}
		}
		RestoreCursor();
		// parent->set_cursor_pos(savedX, savedY);
		parent->SetTextColor(savedTextColor);
		parent->SetBackgroundColor(savedBackgroundColor);
	}
	
	void Window::RestoreCursor() {
		uint16_t newX = MIN(x + cursorX, parent->GetWidth() - 1);
		uint16_t newY = MIN(y + cursorY, parent->GetHeight() - 1);
		parent->SetCursorPos(newX, newY);
	}

	void Window::Resposition(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
		// allocate new buffer
		uint8_t* newScreenBuffer = new uint8_t[width * height * 2];

		uint16_t copyWidth = MIN(width, this->width);
		uint16_t copyHeight = MIN(height, this->height);

		for (uint16_t cy = 0; cy < copyHeight; cy++) {
			for (uint16_t cx = 0; cx < copyWidth; cx++) {
				newScreenBuffer[(cx + cy * width) * 2] = screenBuffer[(cx + cy * this->width) * 2];
				newScreenBuffer[(cx + cy * width) * 2 + 1] = screenBuffer[(cx + cy * this->width) * 2 + 1];
			}
		}

		delete screenBuffer;

		screenBuffer = newScreenBuffer;
		this->width = width;
		this->height = height;
		this->x = x;
		this->y = y;

		if (visible) {
			Redraw();
		}
	}

}
