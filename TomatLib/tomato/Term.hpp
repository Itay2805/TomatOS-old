#ifndef TOMATO_TERM_HPP
#define TOMATO_TERM_HPP

#if !defined(__cplusplus)
	#error("Tomato::Term is a C++ library and can only be included inside a C++ file")
#endif

#include <tomato/term.h>

#include <tomato/Window.hpp>
#include <tomato/Color.hpp>

namespace Tomato {

	class Term {
	friend class Window;
	private:
		Term() {}
		
	public:

		inline static void Write(const char* text) {
			tomato_term_write(text);
		}

		inline static void Clear() {
			tomato_term_clear();
		}
		
		inline static void SetTextColor(Color color) {
			tomato_term_set_text_color((uint8_t)color);
		}

		inline static void SetBackgroundColor(Color color) {
			tomato_term_set_background_color((uint8_t)color);
		}

		inline static Color GetTextColor() {
			return (Color)tomato_term_get_text_color();
		}

		inline static Color GetBackgroundColor() {
			return (Color)tomato_term_get_background_color();
		}

		inline static void SetCursorPos(uint16_t x, uint16_t y) {
			tomato_term_set_cursor_pos(x, y);
		}

		inline static uint16_t GetCursorX() {
			return tomato_term_get_cursor_x();
		}

		inline static uint16_t GetCursorY() {
			return tomato_term_get_cursor_y();
		}

		inline static void Scroll(uint16_t n = 1) {
			tomato_term_scroll(n);
		}

		inline static uint16_t Height() {
			return tomato_term_height();
		}

		inline static uint16_t Width() {
			return tomato_term_width();
		}

		inline static void ClearLine(uint16_t n) {
			tomato_term_clear_line(n);
		}

		inline static void Redirect(Window window) {
			tomato_term_redirect(window.GetHandler());
		}

		inline static Window Current() {
			return Window(tomato_term_current());
		}

		inline static Window Native() {
			return Window(tomato_term_native());
		}
		
	};

}

#endif