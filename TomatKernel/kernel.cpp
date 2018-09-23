#include <core/cpp.hpp>
#include <core/icxxabi.hpp>

#include <string.h>
#include <setjmp.h>

#include <core/process/syscall.h>

extern "C" void kmain() {
	asm volatile
		("int $0x80"
			:
			: "a"(SYSCALL_TERM_CLEAR)
			);

	asm volatile
		("int $0x80"
			:
			: "a"(SYSCALL_TERM_WRITE)
			, "b"("We are now in the main process!\n")
		);

	while (true) {
		// do not exit
		asm("nop");
		asm("hlt");
	}
}