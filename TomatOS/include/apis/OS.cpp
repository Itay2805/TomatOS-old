#include "OS.hpp"

namespace Tomato {

	struct os_event_t {
		uint32_t type;
		size_t data[4];
	};

	void OS::Sleep(float timeout) {
		Timer timer = StartTimer(timeout);
		TimerEvent event;
		do {
			event = PullEvent<TimerEvent>(Event::TIMER);
		} while (event != timer);
	}

	void OS::QueueEvent(uint32_t event, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4) {
		event_t e;

		e.type = event;
		e.data[0] = p1;
		e.data[1] = p2;
		e.data[2] = p3;
		e.data[3] = p4;

		tomato_os_queue_event(&e);
	}

	Timer OS::StartTimer(float timeout) {
		uint32_t millis = timeout * 1000;
		return (Timer)tomato_os_start_timer(millis);
	}

	void OS::CancelTimer(Timer t) {
		tomato_os_cancel_timer((uint32_t)t);
	}

	const char* OS::Version() {
		return tomato_os_version();
	}

}
