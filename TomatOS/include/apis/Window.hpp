#ifndef TOMATO_API_WINDOW_H
#define TOMATO_API_WINDOW_H

#include <cpp.hpp>

#include <apis/Term.hpp>

namespace Tomato {

	// TODO: need to wait until I implement malloc and free again

	class Window : public ITerm {
	public:
		Window();
		~Window();
	};

}

#endif