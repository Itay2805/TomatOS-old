
#include <cpp.hpp>

#include <apis/Term.hpp>
#include <apis/OS.hpp>
#include <apis/Colors.hpp>
#include <apis/Window.hpp>
#include <apis/Coroutine.hpp>
#include <apis/FS.hpp>

using namespace Tomato;

void program(void*) {
	Term::SetBackgroundColor(Colors::LIGHT_BLUE);
	Term::SetTextColor(Colors::WHITE);
	Term::Clear();
	
	Term::Write(OS::Version());
	Term::Write("\n");

	Term::Write("write:\n");
	FS::File file = FS::File("test.txt", FS::File::WRITE, false);
	file.Write("some text", true);
	file.Close();
	Term::Write("finished write\n");

	Term::Write("overwrite:\n");
	file = FS::File("test.txt", FS::File::WRITE, false);
	Term::Write("Size: %i\n", file.GetSize());
	file.Write("this should replace the text in the file", true);
	file.Close();
	Term::Write("finished overwrite\n");

	Term::Write("overwrite smaller:\n");
	file = FS::File("test.txt", FS::File::WRITE, false);
	Term::Write("Size: %i\n", file.GetSize());
	file.Write("OVERRIDED!!!", true);
	file.Close();
	Term::Write("finished overwrite smaller");

	while (true) {
		char c = OS::PullEvent<CharEvent>(Event::CHAR).GetChar();
		char buf[2];
		buf[0] = c;
		buf[1] = 0;
		Term::Write(buf);
	}
}

extern "C" void startup() {
	Coroutine coro(program);
	OS::RunBlockingEventLoop(&coro);
}
