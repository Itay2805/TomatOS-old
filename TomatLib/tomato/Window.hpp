#ifndef TOMATO_WINDOW_HPP
#define TOMATO_WINDOW_HPP

#if !defined(__cplusplus)
#error("Tomato::Window is a C++ library and can only be included inside a C++ file")
#endif

#include <tomato/windows.h>

#include <tomato/Color.hpp>

namespace Tomato {

	class Window {
	private:
		tomato_window_t window;
		
	public:
		inline Window(tomato_window_t window)
			: window(window)
		{
		}

		inline Window(Window parent, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool vis = true)
		{
			window = tomato_window_create(parent.window, x, y, width, height, vis);
		}

		inline bool operator==(const Window& b) {
			return window == b.window;
		}

		inline bool operator!=(const Window& b) {
			return window != b.window;
		}

		inline void Delete() {
			tomato_window_delete(window);
		}

		inline tomato_window_t GetHandler() {
			return window;
		}

		inline void Write(const char* text) {
			tomato_window_write(window, text);
		}

		inline void Clear() {
			tomato_window_clear(window);
		}

		inline void SetTextColor(Color color) {
			tomato_window_set_text_color(window, (uint8_t)color);

		}

		inline void SetBackgroundColor(Color color) {
			tomato_window_set_background_color(window, (uint8_t)color);
		}

		inline Color GetTextColor() {
			return (Color)tomato_window_get_text_color(window);
		}

		inline Color GetBackgroundColor() {
			return (Color)tomato_window_get_background_color(window);
		}

		inline void SetCursorPos(uint16_t x, uint16_t y) {
			tomato_window_set_cursor_pos(window, x, y);
		}

		inline uint16_t GetCursorX() {
			return tomato_window_get_cursor_x(window);
		}

		inline uint16_t GetCursorY() {
			return tomato_window_get_cursor_y(window);
		}

		inline uint16_t Width() {
			return tomato_window_width(window);
		}

		inline uint16_t Height() {
			return tomato_window_height(window);
		}

		inline void Scroll(uint16_t n) {
			tomato_window_scroll(window, n);
		}

		inline void ClearLine(uint16_t n) {
			tomato_window_clear_line(window, n);
		}

		inline void SetVisible(bool vis) {
			tomato_window_set_visible(window, vis);
		}

		inline void Redraw() {
			tomato_window_redraw(window);
		}

		inline uint16_t GetX() {
			return tomato_window_get_x(window);
		}

		inline uint16_t GetY() {
			return tomato_window_get_y(window);
		}

		inline void RestoreCursor() {
			tomato_window_restore_cursor(window);
		}

		inline void Reposition(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
			tomato_window_reposition(window, x, y, width, height);
		}

	};

}

#endif