#include "gdt.h"

static gdt_entry_t entries[] = {
    // GDT_SEGMENT_NULL
    {
        0
    },
    // GDT_SEGMENT_CODE
    {
        .limit_low = 0xFFFF,
        .base_low = 0x000000,
        .accessed = 0,
        .read_write = 1,
        .conforming_expand_down = 0,
        .code = 1,
        .always_1 = 1,
        .DPL = 0,
        .present = 1,
        .limit_high = 0xF,
        .available = 0,
        .always_0 = 0,
        .big = 1,
        .gran = 1,
        .base_high = 0x00
    },
    // GDT_SEGMENT_DATA
    {
        .limit_low = 0xFFFF,
        .base_low = 0x000000,
        .accessed = 0,
        .read_write = 1,
        .conforming_expand_down = 0,
        .code = 0,
        .always_1 = 1,
        .DPL = 0,
        .present = 1,
        .limit_high = 0xF,
        .available = 0,
        .always_0 = 0,
        .big = 1,
        .gran = 1,
        .base_high = 0x00
    },
    // GDT_SEGMENT_USER_CODE
    {
        .limit_low = 0xFFFF,
        .base_low = 0x000000,
        .accessed = 0,
        .read_write = 1,
        .conforming_expand_down = 0,
        .code = 1,
        .always_1 = 1,
        .DPL = 3,
        .present = 1,
        .limit_high = 0xF,
        .available = 0,
        .always_0 = 0,
        .big = 1,
        .gran = 1,
        .base_high = 0x00
    },
    // GDT_SEGMENT_USER_DATA
    {
        .limit_low = 0xFFFF,
        .base_low = 0x000000,
        .accessed = 0,
        .read_write = 1,
        .conforming_expand_down = 0,
        .code = 0,
        .always_1 = 1,
        .DPL = 3,
        .present = 1,
        .limit_high = 0xF,
        .available = 0,
        .always_0 = 0,
        .big = 1,
        .gran = 1,
        .base_high = 0x00
    }
};

gdt_t gdt = { sizeof(gdt_entry_t) *  GDT_ENTRIES, &entries };