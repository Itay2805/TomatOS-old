#ifndef BOOT_GDT_H
#define BOOT_GDT_H

#include <stdint.h>
#include <kernel.h>

#define GDT_ENTRIES 5

#define GDT_SEGMENT_NULL 0
#define GDT_SEGMENT_CODE 8
#define GDT_SEGMENT_DATA 16
#define GDT_SEGMENT_USER_CODE 24
#define GDT_SEGMENT_USER_DATA 32

typedef struct gdt_entry {
    uint64_t limit_low : 16;
	uint64_t base_low : 24;
	//attribute byte split into bitfields
	uint64_t accessed : 1;
	uint64_t read_write : 1; //readable for code, writable for data
	uint64_t conforming_expand_down : 1; //conforming for code, expand down for data
	uint64_t code : 1; //1 for code, 0 for data
	uint64_t always_1 : 1; //should be 1 for everything but TSS and LDT
	uint64_t DPL : 2; //priviledge level
	uint64_t present : 1;
	//and now into granularity
	uint64_t limit_high : 4;
	uint64_t available : 1;
	uint64_t always_0 : 1; //should always be 0
	uint64_t big : 1; //32bit opcodes for code, uint32_t stack for data
	uint64_t gran : 1; //1 to use 4k page addressing, 0 for byte addressing
	uint64_t base_high : 8;
} PACKED gdt_entry_t;

typedef struct gdt_t {
    uint16_t size;
    gdt_entry_t* entries;
} PACKED gdt_t;

extern gdt_t gdt;

// this will setup the GDT itself 
void gdt_init(void);

#endif