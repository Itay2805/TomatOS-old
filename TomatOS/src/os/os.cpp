
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

	Term::Write("Sleeping for 10 seconds");
	OS::Sleep(10);
	Term::Write("Slept for 10 seconds!");

	Timer heyTimer = OS::StartTimer(1);
	while (true) {
		Event event = OS::PullEvent();
		switch (event.GetEventType()) {
			case Event::TIMER: {
				Term::Write("Hey!\n");
				heyTimer = OS::StartTimer(1);
			} break;
		}
	}
}

extern "C" void startup() {
	Coroutine coro(program);
	event_t event;
	while (true) {
		Event::EventType filter = (Event::EventType)coro.Resume<uint32_t, event_t*>(&event);
		if (coro.GetStatus() == Coroutine::DEAD) {
			break;
		}
		tomato_os_pull_event_blocking(&event, filter);
	}
}
