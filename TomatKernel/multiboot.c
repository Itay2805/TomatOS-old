#include "multiboot.h"

#include <stdlib.h>

multiboot_mem_t* multiboot_get_mem(multiboot_t* multiboot) {
    if((multiboot->flags & (1 << 0)) != 0) {
        return (multiboot_mem_t*)((size_t)multiboot + 4);
    }else {
        return 0;
    }
}
