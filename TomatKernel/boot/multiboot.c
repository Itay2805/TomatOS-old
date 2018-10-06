#include "multiboot.h"

static int sizes[] = {
    sizeof(multiboot_mem_t),    // 0
    4,                          // 1
    4,                          // 2
    8,                          // 3
    18,                         // 4
    18,                         // 5
    8,                          // 6
    8,                          // 7            
    4,                          // 8
    4,                          // 9
    4,                          // 10
    16,                         // 11
    sizeof(multiboot_framebuffer_t) // 12
};

static uint32_t get_offset(multiboot_t* multiboot, int bit) {
    int offset = 4;
    for(int i = 0; i < bit; i++) {
        if((multiboot->flags & (1 << 4)) != 0 && i == 5) continue;
        if((multiboot->flags & (1 << i)) != 0) {
            offset += sizes[bit];
        }
    }
    return offset;
}

multiboot_mem_t* multiboot_get_mem(multiboot_t* multiboot) {
    if((multiboot->flags & (1 << 0)) != 0) {
        return (multiboot_mem_t*)((size_t)multiboot + get_offset(multiboot, 0));
    }else {
        return 0;
    }
}

multiboot_framebuffer_t* multiboot_get_framebuffer(multiboot_t* multiboot) {
    if((multiboot->flags & (1 << 12)) != 0) {
        return (multiboot_framebuffer_t*)((size_t)multiboot + get_offset(multiboot, 12));
    }else {
        return 0;
    }
}
