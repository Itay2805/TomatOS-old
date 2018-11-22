#include "pic.h"

#include <stdint.h>
#include <kernel.h>

void pic_remap(uint8_t master_offset, uint8_t slave_offset) {
    // save the mask
    uint8_t master_mask = inb(PIC_MASTER_DATA);
    uint8_t slave_mask = inb(PIC_MASTER_DATA);

    outb(PIC_MASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC_SLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(PIC_MASTER_DATA, master_offset);
    outb(PIC_SLAVE_DATA, slave_offset);

    outb(PIC_MASTER_DATA, 4);
    outb(PIC_SLAVE_DATA, 2);

    outb(PIC_MASTER_DATA, ICW4_8086);   
    outb(PIC_SLAVE_DATA, ICW4_8086);   

    // restore the mask
    outb(PIC_MASTER_DATA, master_mask);
    outb(PIC_SLAVE_DATA, slave_mask);
}

void pic_send_eoi() {

}
