
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>

using namespace Tomato;

void program(void*) {
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();
	
	Term::Write(OS::Version());
	Term::Write("\n");

	while (true) {
		char c = OS::PullEvent<CharEvent>(Event::CHAR).GetChar();
		char buf[2];
		buf[0] = c;
		buf[1] = 0;
		Term::Write(buf);
	}
}

extern "C" void startup() {
	Coroutine coro(program);
	OS::RunBlockingEventLoop(&coro);
}
