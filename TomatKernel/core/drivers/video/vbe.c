#include "vbe.h"

#define DEFAULT_MAX_WIDTH 1024
#define DEFAULT_MAX_HEIGHT 768

static int max_width, max_height, max_bpp;

uint16_t vbe_max_width() {
    return max_width;
}

uint16_t vbe_max_height() {
    return max_height;
}

uint16_t vbe_max_bpp() {
    return max_bpp;
}

void vbe_init(void) {
    uint16_t id = vbe_id();
    if(id >= VBE_VERSION_4) {
        // query GETCAPS
        vbe_enable(VBE_FLAG_ENABLED | VBE_FLAG_GETCAPS);
        max_width = vbe_max_width();
        max_height = vbe_max_height();
        max_bpp = vbe_max_bpp();
        vbe_disable();
    }else {
        max_width = DEFAULT_MAX_WIDTH;
        max_height = DEFAULT_MAX_HEIGHT;
        if(id >= VBE_VERSION_3) {
            max_bpp = 32;
        }else {
            max_bpp = 0;            
        }
    }
}

void vbe_write(uint16_t param, uint16_t value) {
    outw(VBE_INDEX, param);
    outw(VBE_DATA, value);
}

uint16_t vbe_read(uint16_t param) {
    outw(VBE_INDEX, param);
    return inw(VBE_DATA);
}

uint16_t vbe_id() {
    return vbe_read(VBE_ID);   
}

void vbe_set_width(uint16_t width) {
    vbe_write(VBE_WIDTH, width);
}

uint16_t vbe_get_width() {
    return vbe_read(VBE_WIDTH);
}

void vbe_set_height(uint16_t height) {
    vbe_write(VBE_HEIGHT, height);
}

uint16_t vbe_get_height() {
    return vbe_read(VBE_HEIGHT);
}


void vbe_set_bpp(uint16_t bpp) {
    vbe_write(VBE_BPP, bpp);    
}

uint16_t vbe_get_bpp() {
    return vbe_read(VBE_BPP);
}

void vbe_set_bank(uint16_t bank) {
    vbe_write(VBE_BANK, bank);    
}

uint16_t vbe_get_bank() {
    return vbe_read(VBE_BANK);
}

void vbe_enable(uint16_t flags) {
    vbe_write(VBE_ID, VBE_FLAG_ENABLED | flags);
}

void vbe_disable() {
    vbe_write(VBE_ID, VBE_FLAG_DISABLED);
}

void vbe_set_pixel(uint32_t color, uint16_t x, uint16_t y) {
    uint16_t width = vbe_get_width();
    uint16_t bpp = vbe_get_bpp();
    int offset = x + y * width;
    int bank = offset / 64;
    int bank_pixel = offset - bank * 64;
    vbe_set_bank(bank);
    switch(bpp) {
        case 0:
        case 8:
            *((uint8_t*)VBE_BANK_ADDRESS +  bank_pixel) = color;
            break;

        case 16:
            *((uint16_t*)VBE_BANK_ADDRESS +  bank_pixel) = color;
            break;

        case 32:
            *((uint32_t*)VBE_BANK_ADDRESS +  bank_pixel) = color;
            break;

        default:
            kpanic("Invalid BPP for vbe_set_pixel");
            break;
    }
}

void vbe_blit(uint32_t* color, uint16_t blit_x, uint16_t blit_y, uint16_t blit_width, uint16_t blit_height) {
    uint16_t width = vbe_get_width();

    // really not optimal, should make it better
    for(uint32_t y = 0; y < blit_height; y++) {
        for(uint32_t x = 0; x < blit_width; x++) {
            vbe_set_pixel(color[x + y * width], x + blit_x, y + blit_y);
        }
    }
}
