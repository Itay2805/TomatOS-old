#include "multiboot.h"

multiboot_mem_t* multiboot_get_mem(multiboot_t* multiboot) {
    if((multiboot->flags & (1 << 0)) != 0) {
        return (multiboot_mem_t*)((size_t)multiboot + 4);
    }else {
        return 0;
    }
}

multiboot_framebuffer_t* multiboot_get_framebuffer(multiboot_t* multiboot) {
    if((multiboot->flags & (1 << 12)) != 0) {
        return (multiboot_framebuffer_t*)((size_t)multiboot + 88);
    }else {
        return 0;
    }
}
