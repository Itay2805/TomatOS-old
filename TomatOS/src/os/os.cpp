
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

	FS::MakeDir("folder1");
	Term::Write("created folder1\n");

	FS::MakeDir("folder1/subfolder1");
	Term::Write("created folder1/subfolder1\n");
	
	FS::MakeDir("folder1/subfolder2");
	Term::Write("created folder1/subfolder2\n");
	
	FS::MakeDir("folder2");
	Term::Write("created folder2\n");

	Term::Write("files under root: \n");
	FS::List list = FS::List("");
	for (int i = 0; i < list.Count(); i++) {
		FS::File file = FS::File(list[i]);
		Term::Write(list[i]);
		if (file.IsDir()) {
			Term::Write(": \n");
			FS::List sublist = FS::List(list[i]);
			for (int i = 0; i < sublist.Count(); i++) {
				Term::Write("\t");
				Term::Write(sublist[i]);
				Term::Write("\n");
			}
		}
		else {
			Term::Write("\n");
		}
	}

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
