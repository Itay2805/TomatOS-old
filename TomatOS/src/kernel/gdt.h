#ifndef KERNEL_GDT_H
#define KERNEL_GDT_H

#include "common.h"

#include <stdint.h>
#include <stddef.h>

typedef struct gdt_entry_t {
	unsigned int limit_low : 16;
	unsigned int base_low : 24;
	//attribute byte split into bitfields
	unsigned int accessed : 1;
	unsigned int read_write : 1; //readable for code, writable for data
	unsigned int conforming_expand_down : 1; //conforming for code, expand down for data
	unsigned int code : 1; //1 for code, 0 for data
	unsigned int always_1 : 1; //should be 1 for everything but TSS and LDT
	unsigned int DPL : 2; //priviledge level
	unsigned int present : 1;
	//and now into granularity
	unsigned int limit_high : 4;
	unsigned int available : 1;
	unsigned int always_0 : 1; //should always be 0
	unsigned int big : 1; //32bit opcodes for code, uint32_t stack for data
	unsigned int gran : 1; //1 to use 4k page addressing, 0 for byte addressing
	unsigned int base_high : 8;
} PACKED gdt_entry_t;

typedef struct gdt_t {
	gdt_entry_t null;
	gdt_entry_t unused;
	gdt_entry_t code;
	gdt_entry_t data;
} gdt_t;

void initialize_gdt();
gdt_t* get_gdt();

#endif