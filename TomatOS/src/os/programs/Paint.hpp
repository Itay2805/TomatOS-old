#ifndef OS_PROGRAMS_PAINT_HPP
#define OS_PROGRAMS_PAINT_HPP

#include <apis/Term.hpp>
#include <apis/Colors.hpp>
#include <apis/PaintUtils.hpp>

namespace Tomato {

	class Paint {
	private:
		int h, w;
		const char* message;
		Colors::Color canvasColor;
		Colors::Color leftColor;
		Colors::Color rightColor;
		PaintUtils::Image* image;

		bool HasPixel(int x, int y) {
			return image != nullptr && (x > 0 && y > 0 && x < image->GetWidth() && y < image->GetHeight());
		}

		void DrawCanvasPixel(int x, int y) {
			if (HasPixel(x, y)) {
				Term::SetBackgroundColor(image->GetPixel(x, y));
				Term::SetCursorPos(x, y);
				Term::Write(" ");
			}else {
				Term::SetBackgroundColor(canvasColor);
				Term::SetTextColor(Colors::GRAY);
				Term::SetCursorPos(x, y);
				char buf[] = { (char)176, 0 };
				Term::Write(buf);
			}
		}

		void DrawCanvasLine(int y) {
			for (int x = 0; x < w - 2; x++) {
				DrawCanvasPixel(x, y);
			}
		}

		void DrawCanvas() {
			for (int y = 0; y < h; y++) {
				DrawCanvasLine(y);
			}
		}

		void DrawInterface() {
			Term::SetCursorPos(0, h - 1);
			Term::SetBackgroundColor(Colors::BLACK);
			Term::SetTextColor(Colors::YELLOW);
			Term::ClearLine();
			Term::Write(message);

			for (int i = 0; i < 16; i++) {
				Term::SetCursorPos(w - 2, i);
				Term::SetBackgroundColor(i);
				Term::Write("  ");
			}

			Term::SetCursorPos(w - 2, 16);
			Term::SetBackgroundColor(canvasColor);
			Term::SetTextColor(Colors::GRAY);
			char buf[] = { (char)176, (char)176, 0 };
			Term::Write(buf);

			for (int i = 17; i < 19; i++) {
				Term::SetCursorPos(w - 2, i);
				
				Term::SetBackgroundColor(leftColor);
				Term::Write(" ");
				Term::SetBackgroundColor(rightColor);
				Term::Write(" ");
			}

			Term::SetBackgroundColor(canvasColor);
			for (int i = 19; i < h; i++) {
				Term::SetCursorPos(w - 2, i);
				Term::Write("  ");
			}
		}

	public:
		Paint() {
			h = Term::GetHeight();
			w = Term::GetWidth();
			message = "Press Ctrl to access menu";
			canvasColor = Colors::BLACK;
			leftColor = Colors::WHITE;
			rightColor = Colors::BLACK;

			DrawCanvas();
			DrawInterface();
		}

		void Run() {

		}

	};

}

#endif