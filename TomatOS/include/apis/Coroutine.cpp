#include "Coroutine.hpp"


namespace Tomato {

	Coroutine* Coroutine::current = nullptr;
	void* Coroutine::yieldarg = nullptr;

	Coroutine::Coroutine(CoroutineFunc func) 
		: func(func) 
		, status(Status::SUSPENDED)
		, stack(nullptr)
		, prevStack(nullptr)
		, first(true)
	{

	}

	Coroutine::~Coroutine() {
		if (stack != nullptr) {
			free(stack);
		}
	}

	Coroutine::Status Coroutine::GetStatus() {
		return status;
	}

	Coroutine* Coroutine::Current() {
		return current;
	}

}
