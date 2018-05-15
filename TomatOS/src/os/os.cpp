
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
	
	for (int i = 0; i < Term::GetHeight() - 1; i++) {
		Term::Write(OS::Version());
		Term::Write("\n");
	}
	
	OS::Sleep(4);

	Term::SetBackgroundColor(Colors::BLUE);

	Term::Scroll(5);
}

extern "C" void startup() {
	Coroutine coro(program);
	OS::RunBlockingEventLoop(&coro);
}
