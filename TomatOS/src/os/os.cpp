
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>

using namespace Tomato;

static void test(void* test) {
	for (int i = 0; i < 10; i++) {
		Coroutine::Yield(i);
	}
}

extern "C" void startup() {

	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();

	Term::Write(OS::Version());
	Term::Write("\n");

	char buf[3];
	buf[1] = '\n';
	buf[2] = 0;

	Coroutine coro(test);
	while(true) {
		int i = coro.Resume<void*, int>();
		if (coro.GetStatus() == Coroutine::DEAD) {
			break;
		}
		buf[0] = i + '0';
		Term::Write(buf);
	}
}
