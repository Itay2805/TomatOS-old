#include <apis/Term.hpp>

#include <tomato.h>

namespace Tomato {
	
	class NativeTerm : public ITerm {
	private:

	public:
		void Write(const char* text) override {
			tomato_term_write(text);
		}
		
		void Clear() override {
			tomato_term_clear();
		}
		
		void SetTextColor(uint8_t color) override {
			tomato_term_set_text_color(color);
		}
		
		void SetBackgroundColor(uint8_t color) override {
			tomato_term_set_background_color(color);
		}
		
		uint8_t GetTextColor() override {
			return tomato_term_get_text_color();
		}
		
		uint8_t GetBackgroundColor() override {
			return tomato_term_get_text_color();
		}
		
		void SetCursorPos(uint16_t x, uint16_t y) override {
			tomato_term_set_cursor_pos(x, y);
		}
		
		uint16_t GetCursorX() override {
			return tomato_term_get_cursor_x();
		}
		
		uint16_t GetCursorY() override {
			return tomato_term_get_cursor_y();
		}
		
		uint16_t GetWidth() override {
			return tomato_term_get_width();
		}
		
		uint16_t GetHeight() override {
			return tomato_term_get_height();
		}
		
		void Scroll(uint16_t n) override {
			tomato_term_scroll(n);
		}
		
		void ClearLine(uint16_t n) override {
			tomato_term_clear_line(n);
		}

	};

	static NativeTerm native;
	ITerm* Term::current = &native;

	ITerm* Term::Native() {
		return &native;
	}

}
