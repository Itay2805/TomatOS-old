#ifndef TOMATO_OS_HPP
#define TOMATO_OS_HPP

#include <tomato/os.h>

namespace Tomato {
	namespace OS {

		

		class Event {
		protected:
			tomato_event event;

		public:
			enum EventKind {
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

		class TimerEvent : Event {
		public:
			TimerEvent()
			{

			}

			Timer ID() const {
				return event.data[0];
			}
		};

		template<class E = Event>
		static E PullEvent() {
			E event = PullEventRaw();
			if (event.Kind() == Event::TERMINATE) {
				// terminate process
				tomato_os_kill(0);
			}
			return event;
		}

		template<class E = Event>
		static E PullEventRaw() {
			E e;
			tomato_os_pull_event(e.Raw());
			return e;
		}

		static inline void QueueEvent(Event event, uint32_t uid = 0) {
			tomato_os_queue_event(event.Raw(), uid);
		}

	}
}

#endif