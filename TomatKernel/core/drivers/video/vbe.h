#ifndef KERNEL_CORE_DRIVERS_VIDEO_VBE_H
#define KERNEL_CORE_DRIVERS_VIDEO_VBE_H

#include <stdint.h>
#include <stdbool.h>

#include <kernel.h>

#define VBE_BANK_ADDRESS 0xA0000

#define VBE_INDEX 0x1CE
#define VBE_DATA 0x1CF

// 0xb0c0
#define VBE_VERSION_1 0xB0C0
#define VBE_ID 0x0
#define VBE_WIDTH 0x1
#define VBE_HEIGHT 0x2
#define VBE_BPP 0x3
#define VBE_ENABLE 0x4
#define VBE_BANK 0x5

// 0xb0c1
#define VBE_VERSION_2 0xB0C1

#define VBE_VIRTUAL_WIDTH 0x6
#define VBE_VIRTUAL_HEIGHT 0x7
#define VBE_X_OFFSET 0x8
#define VBE_Y_OFFSET 0x9

#define VBE_FLAG_ENABLED 0x00
#define VBE_FLAG_DISABLED 0x01

// 0xb0c2
#define VBE_VERSION_3 0xB0C2

#define VBE_FLAG_NOCLEARMEM 0x80
#define VBE_FLAG_LFB_ENABLED 0x40

// 0xb0c3
#define VBE_VERSION_4 0xB0C3 

#define VBE_FLAG_GETCAPS 0x02
#define VBE_FLAG_8BIT_DAC 0x20

// 0xb0c4
#define VBE_VERSION_5 0xB0C4

#define VBE_VIDEO_MEMORY_64k 0xa

#ifdef __cplusplus
extern "C" {
#endif

    void vbe_init(void);

    void vbe_write(uint16_t param, uint16_t value);
    uint16_t vbe_read(uint16_t param);

    uint16_t vbe_id();

    void vbe_set_width(uint16_t width);
    uint16_t vbe_get_width();

    void vbe_set_height(uint16_t height);
    uint16_t vbe_get_height();

    void vbe_set_bpp(uint16_t bpp);
    uint16_t vbe_get_bpp();

    void vbe_set_bank(uint16_t bank);
    uint16_t vbe_get_bank();

    void vbe_enable(uint16_t flags);
    void vbe_disable();

    void vbe_set_pixel(uint32_t color, uint16_t x, uint16_t y);
    void vbe_blit(uint32_t* color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    uint16_t vbe_max_width();
    uint16_t vbe_max_height();
    uint16_t vbe_max_bpp();

#ifdef __cplusplus
}
#endif
#endif