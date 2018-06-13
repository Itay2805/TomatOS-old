
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
	
	FS::File file = FS::File("test.txt", FS::File::WRITE, false);
	file.Write("some text", true);
	file.Close();

	file = FS::File("test.txt", FS::File::READ);
	char* buffer = new char[file.GetSize()];
	file.ReadAll(buffer, file.GetSize());
	Term::Write("content: %s\n", buffer);
	delete[] buffer;

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
