
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
	
	//Paint paint = Paint();

	//paint.Run();

	while (true) {
		MouseClickEvent e = OS::PullEvent<MouseClickEvent>(Event::MOUSE_CLICK);
		Term::Write("button: %i, x: %i, y: %i\n", (uint32_t)e.GetMouseButton(), (uint32_t)e.GetX(), (uint32_t)e.GetY());
	}

}

extern "C" void startup() {
	Coroutine coro(program);
	OS::RunBlockingEventLoop(&coro);
}
