#include <tomato/Heap.hpp>
#include <icxxabi.hpp>

#include <string.h>

#include <tomato/Term.hpp>
#include <tomato/OS.hpp>

extern "C" void kmain() {
	using namespace Tomato;

	Term::Clear();
	Term::SetCursorPos(0, 0);
	Term::Write("Inside process!\n");

	Timer timer = OS::StartTimer(1.0);

	while (true) {
		Event event = OS::PullEvent();
		Term::Write("Got Event!\n");
		if(event.Kind() == Event::TIMER) {
			TimerEvent t = OS::CastEvent<TimerEvent>(event);
			if(t == timer) {
				Term::Write("Timer Finished!\n");
			}
		}
	}
}