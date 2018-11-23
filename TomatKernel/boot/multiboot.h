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

typedef struct multiboot_framebuffer_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} multiboot_framebuffer_color_t;

typedef struct multiboot_framebuffer {
    uint64_t vram_addr;
    
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;

    // 0 = Pallete, 1 = RGB
    uint8_t type;
    union {
        struct {
            multiboot_framebuffer_color_t* colors;
            uint16_t length;
        } pallete;
        struct {
            uint8_t red_field_position;
            uint8_t red_mask_size;
            uint8_t green_field_position;
            uint8_t green_mask_size;
            uint8_t blue_field_position;
            uint8_t blue_mask_size;
        } rgb;
    } color_info;

} multiboot_framebuffer_t;

#ifdef __cplusplus
extern "C" {
#endif

multiboot_mem_t* multiboot_get_mem(multiboot_t* multiboot);
multiboot_framebuffer_t* multiboot_get_framebuffer(multiboot_t* multiboot);

#ifdef __cplusplus
}
#endif
#endif