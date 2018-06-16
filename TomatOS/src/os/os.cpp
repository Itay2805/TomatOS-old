
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>
#include <apis/FS.hpp>

using namespace Tomato;

static bool exit;

void PrintError(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	uint8_t oldColor = Term::GetTextColor();
	Term::SetTextColor(Colors::RED);
	vprintf(format, arg);
	Term::SetTextColor(oldColor);
	va_end(arg);
}

void shell(void*) {
	Term::SetBackgroundColor(Colors::BLACK);
	Term::SetTextColor(Colors::YELLOW);
	Term::Write("%s\n", OS::Version());
	Term::SetBackgroundColor(Colors::WHITE);

	exit = false;

	ITerm* parent = Term::Current();

	const char* path = "";

	while (!exit) {
		Term::Redirect(parent);
		Term::SetBackgroundColor(Colors::BLACK);
		Term::SetTextColor(Colors::YELLOW);
		Term::Write("%s> ", path);
		Term::SetTextColor(Colors::WHITE);

		char line[256];
		int len = 0;
		uint8_t key;
		while(true) {
			Event e = OS::PullEvent();
			if (e.GetEventType() == Event::KEY) {
				KeyEvent* k = ((KeyEvent*)&e);
				if (k->GetKeyCode() == TOMATO_KEYS_ENTER && !k->IsHeld()) {
					break;
				}
				// check backspace
			}
			else if (e.GetEventType() == Event::CHAR) {
				char c = ((CharEvent*)&e)->GetChar();
				Term::Write("%c", c);
				line[len++] = c;
			}
		};
		line[len] = 0;
		Term::Write("\n");

		// TODO: maybe make all commands programs
		int w = Term::GetWidth();
		int h = Term::GetHeight();

		if (strcmp(line, "EXIT") == 0) {
			exit = true;
		}
		else if (strcmp(line, "CLEAR") == 0) {
			Term::Clear();
			Term::SetCursorPos(1, 1);
		}
		else {
			PrintError("No such command or program\n");
		}
	}
}

extern "C" void startup() {
	Term::Clear();

	Coroutine coro(shell);
	
	Event event;
	while (true) {
		Event::EventType filter = (Event::EventType)coro.Resume<uint32_t, Event*>(&event);
		if (coro.GetStatus() == Coroutine::DEAD) {
			break;
		}
		event = OS::PullEventBlocking(filter);
	}
	
	Term::Redirect(Term::Native());
	Term::Write("Press any key to continue");
	OS::PullEventBlocking(Event::KEY);
	// shutdown
}
