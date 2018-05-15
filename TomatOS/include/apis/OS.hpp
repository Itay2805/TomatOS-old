#ifndef TOMATO_APIS_OS_HPP
#define TOMATO_APIS_OS_HPP

#include <tomato.h>

#include <apis/Coroutine.hpp>

namespace Tomato {

	typedef uint32_t Timer;

	class Event {
	protected:
		uint32_t type;
		uint32_t data[4];

	public:
		enum EventType : uint32_t {
			ALL				= TOMATO_EVENT_ALL,
			TERMINATE		= TOMATO_EVENT_TERMINATE,
			TIMER			= TOMATO_EVENT_TIMER,
			CHAR			= TOMATO_EVENT_CHAR,
			KEY				= TOMATO_EVENT_KEY,
			KEY_UP			= TOMATO_EVENT_KEY_UP,
			EXCEPTION		= TOMATO_EVENT_EXCEPTION
		};

		Event() {}

		Event(uint32_t type, uint32_t* data)
			: type(type)
		{
			this->data[0] = data[0];
			this->data[1] = data[1];
			this->data[2] = data[2];
			this->data[3] = data[3];
		}

		EventType GetEventType() {
			return (EventType)type;
		}

		uint32_t* GetRawData() {
			return data;
		}

	};

	class TimerEvent : public Event {
	public:
		
		Timer GetTimer() {
			return (Timer)data[0];
		}

		bool operator==(Timer t) {
			return t == GetTimer();
		}

		bool operator!=(Timer t) {
			return t != GetTimer();
		}

	};

	class ExceptionEvent : public Event {
	public:
		enum ExceptionType : size_t {
		};

		ExceptionType GetExceptionType() {
			return (ExceptionType)data[0];
		}

		const char* GetMessage() {
			return (const char*)data[1];
		}

	};

	class CharEvent : public Event {
	public:
		char GetChar() {
			return (char)data[0];
		}
	};

	class KeyEvent : public Event {
	public:
		uint8_t GetKeyCode() {
			return (uint8_t)data[0];
		}
		
		bool IsHeld() {
			return (bool)data[1];
		}
	};

	class KeyUpEvent : public Event {
	public:
		uint8_t GetKeyCode() {
			return (uint8_t)data[0];
		}
	};

	class OS {
	private:
		OS() {}

	public:

		template<class E = Event>
		static E PullEventBlocking(Event::EventType filter = Event::ALL) {
			event_t native_event;
			while (!tomato_os_pull_event(&native_event, (uint32_t)filter)) {
				asm("nop");
			}
			Event event(native_event.type, native_event.data);
			E* casted = (E*)&event;
			return *casted;
		}

		// for now the pull event raw will be blocking 
		template<class E = Event>
		static E PullEventRaw(Event::EventType filter = Event::ALL) {
			Event* event = Coroutine::Yield<Event*>(filter);
			E* casted = (E*)event;
			return *casted;
		}

		template<class E = Event>
		static E PullEvent(Event::EventType filter = Event::ALL) {
			// TODO: make so this will terminate on exception or event
			// will only be able to do it when we have a process or something
			return PullEventRaw<E>(filter);
		}

		static void RunBlockingEventLoop(Coroutine* coro);

		static void QueueEvent(uint32_t type, uint32_t p1 = 0, uint32_t p2 = 0, uint32_t p3 = 0, uint32_t p4 = 0);

		static Timer StartTimer(float timeout);
		static void CancelTimer(Timer timer);
		
		static void Sleep(float timeout);

		static const char* Version();

	};

}

#endif