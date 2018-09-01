#include <core/term.h>

#include <core/cpp.hpp>
#include <core/icxxabi.hpp>

#include <string.h>
#include <setjmp.h>

#include <core/process/process.h>

void printError(const char* status, const char* message) {
	uint8_t c = term_get_text_color();
	term_set_text_color(COLOR_RED);
	term_write("[");
	term_write(status);
	term_write("] ");
	term_write(message);
	term_write("\n");
	term_set_text_color(c);
}

extern "C" void kmain() {
	term_clear();

	term_write("Welcome to TomatOS!\n");

	process_t process;
	process.process_size = 0;
	process_create(&process);

	paging_set(process.pd);
	paging_enable();

	term_write("we are using paging now \\o/\n");
}