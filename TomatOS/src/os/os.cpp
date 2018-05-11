
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/Colors.h>


extern "C" void startup() {

	using namespace Tomato;
	Term::SetBackgroundColor(Tomato::Colors::LIGHT_BLUE);
	Term::Clear();
	Term::Write("Hello World!\n");
	Term::Write("Another test!\n");

}
