#include <core/cpp.hpp>
#include <core/icxxabi.hpp>

#include <string.h>

#include <tomato/Term.hpp>
#include <tomato/OS.hpp>

extern "C" void kmain() {
	using namespace Tomato;

	Term::Clear();
	Term::SetCursorPos(0, 0);
	Term::Write("Inside process!\n");

	while (true) {
		Event event = OS::PullEvent();
		Term::Write("Got Event!\n");
	}
}