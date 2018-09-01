#include "gdt.h"

#include <core/term.h>

gdt_t gdt;
static gdt_entry_t gdt_entries[GDT_ENTRIES];
static void gdt_entry_set(uint32_t index, uint32_t base, uint32_t limit, gdt_entry_t access);

void gdt_init() {
	term_write("[gdt] Initializing\n");

    // set gdt
    gdt.entries = gdt_entries;
    gdt.size = sizeof(gdt_entries);

    // null section
    gdt_entry_t entry = {0};
    gdt_entry_set(GDT_SEGMENT_NULL, 0, 0, entry);

	// TODO: set up the limits and bases currectly according to the paging

    // kernel sections
    entry.accessed = 0;
    entry.read_write = 1;
    entry.conforming_expand_down = 0;
    entry.code = 1;
    entry.DPL = 0;
    entry.present = 1;
    entry.available = 0;
    entry.big = 1;
    entry.gran = 1;
    gdt_entry_set(GDT_SEGMENT_CODE, 0, 0xFFFFFFFF, entry);
    entry.code = 0;
    gdt_entry_set(GDT_SEGMENT_DATA, 0, 0xFFFFFFFF, entry);
    
    // user sections - ring3
    entry.DPL = 3;
    gdt_entry_set(GDT_SEGMENT_USER_CODE, 0, 0xFFFFFFFF, entry);
    entry.code = 1;
    gdt_entry_set(GDT_SEGMENT_USER_DATA, 0, 0xFFFFFFFF, entry);

    gdt_t* gdtptr = &gdt;
    // flush gdt

#ifndef _MBCS
    asm volatile(
        "lgdt (%0)"
        :
        : "p"(gdtptr)
    );
#endif
    
    gdt_set_segments();
}

static void gdt_entry_set(uint32_t index, uint32_t base, uint32_t limit, gdt_entry_t access) {
    gdt_entry_t* entry = &gdt.entries[index / 8];
    *entry = access;
    entry->always_0 = 0;
    entry->always_1 = 1;

    entry->base_low = base & 0xFFFFFF;
    entry->base_high = (base >> 24) & 0xFF;

    entry->limit_low = limit & 0xFFFF;
    entry->limit_high = (limit >> 16) & 0x0F;
}
