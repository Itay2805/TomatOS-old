
#include <apis/Term.hpp>

extern "C" void startup() {

	using namespace Tomato;

	Term::SetBackgroundColor(0x2);
	Term::Clear();

	Term::Write("Hello World!\n");

}
