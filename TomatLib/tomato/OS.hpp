#ifndef TOMATO_OS_HPP
#define TOMATO_OS_HPP

#if !defined(__cplusplus)
	#error("Tomato::OS is a C++ library and can only be included inside a C++ file")
#endif

#include <tomato/os.h>

namespace Tomato {

	class Event {
	protected:
		tomato_event event;

	public:
		enum EventKind {
			ANY = TOMATO_EVENT_ANY,
			TERMINATE = TOMATO_EVENT_TERMINATE,
			TIMER = TOMATO_EVENT_TIMER
		};

		Event() 
		{

		}

		tomato_event_t* Raw() {
			return &event;
		}

		EventKind Kind() const {
			return (EventKind)event.kind;
		}
	};

	typedef uint32_t Timer;

	class OS {
	private:
		OS() {}

	public:
		class TimerEvent : Event {
		public:
			TimerEvent()
			{

			}

			Timer ID() const {
				return event.data[0];
			}

			bool operator==(Timer& timer) {
				return ID() == timer;
			}

			bool operator!=(Timer& timer) {
				return ID() != timer;
			}
		};

		static Timer StartTimer(float millis) {
			// TODO:
		}

		static void CancelTimer(Timer timer) {
			// TODO:
		}

		static void Sleep(float millis) {
			Timer timer = StartTimer(millis);
			TimerEvent event;
			do {
				event = PullEvent<TimerEvent>(Event::TIMER);
			} while (event != timer); 
			// technically a bad idea since it will destroy any other timer events
		}

		template<class E = Event>
		static E PullEvent(Event::EventKind kind = Event::ANY) {
			Event event = PullEventRaw(kind);
			E* ep = (E*)&event;
			if (event.Kind() == Event::TERMINATE) {
				// terminate process
				tomato_os_kill(0);
			}
			return *ep;
		}

		template<class E = Event>
		static E PullEventRaw(Event::EventKind kind = Event::ANY) {
			tomato_event_t event;
			do {
				tomato_os_pull_event(&event);
			} while (event.kind != (uint32_t)kind);
			E e;
			*e.Raw() = event;
			return e;
		}

		static inline void QueueEvent(Event event, uint32_t uid = 0) {
			tomato_os_queue_event(event.Raw(), uid);
		}

	};
}

#endif