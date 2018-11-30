#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

typedef struct multiboot {
    uint8_t flags; 
} multiboot_t;

typedef struct multiboot_mem {
    // from 0 to 1mb
    uint32_t lower;
    // from 1mb+
    uint32_t upper;
} multiboot_mem_t;

multiboot_mem_t* multiboot_get_mem(multiboot_t* multiboot);

#endif