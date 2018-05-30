
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>

#include <Pair.hpp>
#include <Typedefs.hpp>
#include <VectorSorted.hpp>

using namespace Tomato;
using namespace cpplib;

void program(void*) {
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();
	
	Term::Write(OS::Version());
	Term::Write("\n");
	
	Status st;

	VectorSorted<int> strings;

	st = strings.Insert(3);
	Term::Write(ErrorMessages[st]);
	Term::Write("\n");

	st = strings.Insert(1);
	Term::Write(ErrorMessages[st]);
	Term::Write("\n");

	st = strings.Insert(2);
	Term::Write(ErrorMessages[st]);
	Term::Write("\n");

	st = strings.Insert(4);
	Term::Write(ErrorMessages[st]);
	Term::Write("\n");

	st = strings.Insert(0);
	Term::Write(ErrorMessages[st]);
	Term::Write("\n");

	Term::Write("================\n");

	VectorCursor<int> cursor(strings);

	for (; cursor.IsValid(); ++cursor) {
		char buf[2];
		buf[0] = cursor.Item() + '0';
		buf[1] = 0;
		Term::Write(buf);
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
