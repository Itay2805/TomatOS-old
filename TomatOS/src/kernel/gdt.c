#include "gdt.h"

static gdt_t gdt;

gdt_t* get_gdt() {
	return &gdt;
}

typedef struct gdt_descriptor_t {
	uint16_t size;
	uint32_t start;
} PACKED gdt_descriptor_t;

void initialize_gdt() {
	uint32_t base = 0;
	uint32_t limit = 64 * 1024 * 1024;

	// set code sections
	gdt.code.limit_low = 0xFFFF; // limit & 0xFFFF;
	gdt.code.base_low = 0;       // base & 0xFFFFFF;

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
	gdt.code.big = 1;
	gdt.code.gran = 1;

	gdt.code.limit_high = 0xF; //(limit >> 16) & 0xF;
	gdt.code.base_high = 0;    //(base >> 24) & 0xFF;
	
	// set data segment, copy from code and set code to false
	gdt.data = gdt.code;
	gdt.data.code = 0;

	// install gdts
	gdt_descriptor_t i;
	i.size = sizeof(gdt_t);
	i.start = (size_t)&gdt;
	ASM(asm volatile("lgdt (%0)" : : "p"(&i)));

}
