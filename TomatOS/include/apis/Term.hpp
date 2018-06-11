#ifndef TOMATO_API_TERM_HPP
#define TOMATO_API_TERM_HPP

#include <tomato.h>
#include <stdint.h>
#include <stddef.h>

namespace Tomato {

	class ITerm {
	public:
		virtual void Write(const char* text) { UNUSED(text); }
		virtual void Clear() {  }
		virtual void SetTextColor(uint8_t color) { UNUSED(color); }
		virtual void SetBackgroundColor(uint8_t color) { UNUSED(color); }
		virtual uint8_t GetTextColor() { return 0; }
		virtual uint8_t GetBackgroundColor() { return 0; }
		virtual void SetCursorPos(uint16_t x, uint16_t y) { UNUSED(x); UNUSED(y); }
		virtual uint16_t GetCursorX() { return 0; }
		virtual uint16_t GetCursorY() { return 0; }
		virtual uint16_t GetWidth() { return 0; }
		virtual uint16_t GetHeight() { return 0; }
		virtual void Scroll(uint16_t n) { UNUSED(n); }
		virtual void ClearLine(uint16_t n) { UNUSED(n); }
	};

	class Term {

	private:
		static ITerm* current;

	public:

		static inline void Write(const char* text)				{ current->Write(text); }
		static inline void Write(uint32_t number)				{ char buf[11]; itoa(number, buf, 10); current->Write(buf); }
		static inline void Clear()								{ current->Clear(); }
		static inline void SetTextColor(uint8_t color)			{ current->SetTextColor(color); }
		static inline void SetBackgroundColor(uint8_t color)	{ current->SetBackgroundColor(color); }
		static inline uint8_t GetTextColor()					{ return current->GetTextColor(); }
		static inline uint8_t GetBackgroundColor()				{ return current->GetBackgroundColor(); }
		static inline void SetCursorPos(uint16_t x, uint16_t y) { current->SetCursorPos(x, y); }
		static inline uint16_t GetCursorX()						{ return current->GetCursorX(); }
		static inline uint16_t GetCursorY()						{ return current->GetCursorY(); }
		static inline uint16_t GetWidth()						{ return current->GetWidth(); }
		static inline uint16_t GetHeight()						{ return current->GetHeight(); }
		static inline void Scroll(uint16_t n)					{ current->Scroll(n); }
		static inline void ClearLine(uint16_t n)				{ current->ClearLine(n); }

		static inline ITerm* Redirect(ITerm* term) { ITerm* old = current;  current = term; return old; }
		static inline ITerm* Current() { return current; }
		static ITerm* Native();
	};

}

#endif