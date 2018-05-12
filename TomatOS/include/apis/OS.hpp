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
			ALL				= 0,
			TERMINATE		= 1,
			TIMER			= 2,
			CHAR			= 3,
			KEY				= 4,
			KEY_UP			= 5,
			EXCEPTION		= 6
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
			DIVISION_BY_ZERO,
			DEBUG_SINGLE_STEP,
			NON_MASKABLE_INTERRUPT,
			BREAKPOINT,
			INTO_DETECTED_OVERFLOW,
			OUT_OF_BOUNDS,
			INVALID_OPCODE,
			NO_COPROCESSOR,
			DOUBLE_FAULT,
			BAD_TSS,
			SEGMENT_NOT_PRESENT,
			GENERAL_PROTECTION_FAULT,
			PAGE_FAULT,
			UNKNOWN_INTERRUPT,
			COPROCESSOR_FAULT,
			ALIGNMENT_CHECK,
			MACHINE_CHECK
		};

		ExceptionType GetExceptionType() {
			return (ExceptionType)data[0];
		}

		const char* GetMessage() {
			return (const char*)data[0];
		}

	};

	class OS {
	private:

	public:

		// for now the pull event raw will be blocking 
		template<class E = Event>
		static E PullEventRaw(Event::EventType filter = Event::ALL) {
			event_t* native_event = Coroutine::Yield<event_t*>(filter);
			Event event(native_event->type, native_event->data);
			E* casted = (E*)&event;
			return *casted;
		}

		template<class E = Event>
		static E PullEvent(Event::EventType filter = Event::ALL) {
			// TODO: make so this will terminate on exception or event
			// will only be able to do it when we have a process or something
			return PullEventRaw<E>(filter);
		}

		static void QueueEvent(uint32_t type, uint32_t p1 = 0, uint32_t p2 = 0, uint32_t p3 = 0, uint32_t p4 = 0);

		Timer StartTimer(float timeout);
		void CancelTimer(Timer timer);
		
		void Sleep(float timeout);

		static const char* Version();

	};

}

#endif