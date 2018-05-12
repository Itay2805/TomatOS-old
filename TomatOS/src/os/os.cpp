
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.h>


extern "C" void startup() {

	using namespace Tomato;
	
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();

	const char* version = OS::Version();
	Term::Write(version);

}
