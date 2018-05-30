
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>

#include <Pair.hpp>
#include <Typedefs.hpp>

using namespace Tomato;
using namespace cpplib;

void program(void*) {
	Pair<CStr> test("test1", "test2");

	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();
	
	Term::Write(OS::Version());
	Term::Write("\n");
	Term::Write("!@#$%^&*()\n");

	Term::Write("Pair: <");
	Term::Write(test.X());
	Term::Write(", ");
	Term::Write(test.Y());
	Term::Write(">\n");

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
