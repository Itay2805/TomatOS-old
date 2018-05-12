
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.h>
#include <apis/Window.hpp>


extern "C" void startup() {

	using namespace Tomato;
	
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();

	Window window(Term::Current(), 10, 10, 10, 10, true);
	ITerm* old = Term::Redirect((ITerm*)&window);

	Term::SetBackgroundColor(Colors::RED);
	Term::Clear();

	const char* version = OS::Version();
	Term::Write(version);

	Term::Redirect(old);
}
