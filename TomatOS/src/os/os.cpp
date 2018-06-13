
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>
#include <apis/FS.hpp>
#include <apis/PaintUtils.hpp>

#include "programs/Paint.hpp"

using namespace Tomato;

void program(void*) {
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();
	
	Paint paint = Paint();

	paint.Run();

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
