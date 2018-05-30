
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>

#include <Pair.hpp>
#include <Typedefs.hpp>
#include <Vector.hpp>

using namespace Tomato;
using namespace cpplib;

void program(void*) {
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();
	
	Term::Write(OS::Version());
	Term::Write("\n");
	
	Vector<CStr> strings;
	strings.Append("string 1");
	strings.Append("string 2");
	strings.Append("string 3");

	VectorCursor<CStr> cursor(strings);

	for (; cursor.IsValid(); ++cursor) {
		Term::Write(cursor.Item());
		Term::Write("\n");
	}

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
