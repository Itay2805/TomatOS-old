#ifndef TOMATO_API_WINDOW_HPP
#define TOMATO_API_WINDOW_HPP

#include <cpp.hpp>

#include <apis/Term.hpp>

namespace Tomato {

	// TODO: need to wait until I implement malloc and free again

	class Window : public ITerm {
	private:
		ITerm * parent;
		uint16_t x, y;
		uint16_t width, height;
		uint16_t cursorX, cursorY;
		uint8_t bgColor, fgColor;
		bool visible;
		uint8_t* screenBuffer;

	public:
		Window(ITerm* parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible);
		~Window();

		void Write(const char* text) override;
		void Clear() override;
		void SetTextColor(uint8_t color) override;
		void SetBackgroundColor(uint8_t color) override;
		uint8_t GetTextColor() override;
		uint8_t GetBackgroundColor() override;
		void SetCursorPos(uint16_t x, uint16_t y) override;
		uint16_t GetCursorX() override;
		uint16_t GetCursorY() override;
		uint16_t GetWidth() override;
		uint16_t GetHeight() override;
		void Scroll(uint16_t n) override;
		void ClearLine(uint16_t n) override;

		void SetVisible(bool vis);
		void Redraw();
		uint16_t GetX();
		uint16_t GetY();
		void RestoreCursor();
		void Resposition(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
	};

}

#endif