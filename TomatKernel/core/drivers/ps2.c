#include "ps2.h"

ps2_status_t ps2_status(void) {
    return *((ps2_status_t*)inb(PS2_COMMAND));
}

void ps2_command(uint8_t command) {
    while(!ps2_status().output_ready) {
        asm("nop");
    }    
    outb(PS2_COMMAND, command);
}

void ps2_write(uint8_t value) {
    while(!ps2_status().output_ready) {
        asm("nop");
    }
    outb(PS2_DATA, value);
}

uint8_t ps2_read(void) {
    while(!ps2_status().input_ready) {
        asm("nop");
    }
    return inb(PS2_DATA);
}
