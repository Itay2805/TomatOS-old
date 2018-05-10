#include "gdt.h"

static gdt_t gdt;

gdt_t* get_gdt() {
	return &gdt;
}

void initialize_gdt() {
	uint32_t base = 0;
	uint32_t limit = 64 * 1024 * 1024;

	// set code sections
	gdt.code.limit_low = limit & 0xFFFF;
	gdt.code.base_low = base & 0xFFFFFF;

	// type
	gdt.code.accessed = 0;
	gdt.code.read_write = 1;
	gdt.code.conforming_expand_down = 0;
	gdt.code.code = 1;
	gdt.code.always_1 = 1;
	gdt.code.DPL = 0;
	gdt.code.present = 1;
	
	gdt.code.available = 0;
	gdt.code.always_0 = 0;
	if (limit <= UINT16_MAX) {
		// 16bit address space
		gdt.code.big = 1;
		gdt.code.gran = 0;
	}
	else {
		// 32bit address space
		gdt.code.big = 1;
		gdt.code.gran = 1;
	}

	gdt.code.limit_high = (limit >> 16) & 0xF;
	gdt.code.base_high = (base >> 24) & 0xFF;
	
	// set data segment, copy from code and set code to false
	gdt.data = gdt.code;
	gdt.data.code = 0;

	// install gdts
	size_t i[2];
	i[1] = (size_t)&gdt;
	i[0] = sizeof(gdt_t) << 16;
	ASM(asm volatile("lgdt (%0)" : : "p"(((uintptr_t)i) + 2)));
}
