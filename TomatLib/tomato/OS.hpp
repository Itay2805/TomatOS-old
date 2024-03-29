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
			CHAR = TOMATO_EVENT_CHAR,
			KEY = TOMATO_EVENT_KEY,
			KEY_UP = TOMATO_EVENT_KEY_UP,
			TIMER = TOMATO_EVENT_TIMER,
			TERMINATE = TOMATO_EVENT_TERMINATE,
			MOUSE_CLICK = TOMATO_EVENT_MOUSE_CLICK,
			MOUSE_UP = TOMATO_EVENT_MOUSE_UP,
			MOUSE_SCROLL = TOMATO_EVENT_MOUSE_SCROLL,
			MOUSE_DRAG = TOMATO_EVENT_MOUSE_DRAG,
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

		bool operator[](uint32_t index) {
			return event.data[index];
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

			bool operator==(Timer& timer) {
				return ID() == timer;
			}

			bool operator!=(Timer& timer) {
				return ID() != timer;
			}
	};

	class KeyEvent : Event {
		public:
			KeyEvent()
			{

			}

			uint32_t KeyCode() const {
				return event.data[0];
			}

			bool Repeat() const {
				return event.data[1];
			}

			bool operator==(uint32_t value) {
				return KeyCode() == value;
			}

			bool operator!=(uint32_t value) {
				return KeyCode() != value;
			}

			bool operator>(uint32_t value) {
				return KeyCode() > value;
			}

			bool operator>=(uint32_t value) {
				return KeyCode() >= value;
			}

			bool operator<(uint32_t value) {
				return KeyCode() < value;
			}

			bool operator<=(uint32_t value) {
				return KeyCode() <= value;
			}
	};

	class KeyUpEvent : Event {
		public:
			KeyUpEvent()
			{

			}

			uint32_t KeyCode() const {
				return event.data[0];
			}

			bool operator==(uint32_t value) {
				return KeyCode() == value;
			}

			bool operator!=(uint32_t value) {
				return KeyCode() != value;
			}

			bool operator>(uint32_t value) {
				return KeyCode() > value;
			}

			bool operator>=(uint32_t value) {
				return KeyCode() >= value;
			}

			bool operator<(uint32_t value) {
				return KeyCode() < value;
			}

			bool operator<=(uint32_t value) {
				return KeyCode() <= value;
			}
	};

	class CharEvent : Event {
		public:
			CharEvent()
			{

			}

			char Char() const {
				return event.data[0];
			}

			bool operator==(char timer) {
				return Char() == timer;
			}

			bool operator!=(char timer) {
				return Char() != timer;
			}

			bool operator>(char timer) {
				return Char() > timer;
			}

			bool operator>=(char timer) {
				return Char() >= timer;
			}

			bool operator<(char timer) {
				return Char() < timer;
			}

			bool operator<=(char timer) {
				return Char() <= timer;
			}
	};

	class OS {
	private:
		OS() {}

	public:

		static Timer StartTimer(float seconds) {
			return (Timer)tomato_os_start_timer((uint32_t)(seconds * 1000));
		}

		static void CancelTimer(Timer timer) {
			tomato_os_cancel_timer((uint32_t)timer);
		}

		static void Kill(uint32_t uid = 0) {
			tomato_os_kill(uid);	
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
			if(kind == Event::ANY) {
				// any event is good
				tomato_os_pull_event(&event);
			}else {
				// we only want events of the specified type
				do {
					tomato_os_pull_event(&event);
				} while (event.kind != (uint32_t)kind);
			}
			E e;
			*e.Raw() = event;
			return e;
		}

		template<class E = Event>
		static inline E CastEvent(Event e) {
			return *((E*)&e);
		}

		static inline void QueueEvent(Event event, uint32_t uid = 0) {
			tomato_os_queue_event(event.Raw(), uid);
		}

	};
}

#endif