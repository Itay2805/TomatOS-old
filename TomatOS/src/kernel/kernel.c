
#include <stdint.h>

#include "gdt.h"

void kmain(const void* multiboot_structure, uint32_t multiboot_magic) {
	initialize_gdt();

	

}