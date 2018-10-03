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
		switch(event.Kind()) {
			case Event::TIMER: {
				TimerEvent t = OS::CastEvent<TimerEvent>(event);
				if(t == timer) {
					Term::Write("One second passed!\n");
				}
			} break;
			case Event::CHAR: {
				CharEvent ch = OS::CastEvent<CharEvent>(event);
				char buf[2];
				buf[0] = ch.Char();
				buf[1] = 0;
				Term::Write(buf);
			} break;
		}
	}
}