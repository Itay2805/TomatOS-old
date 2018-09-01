#include "pic.h"

#include <kernel.h>

void pic_init() {
    outb(PIC_MASTER_COMMAND_PORT, 0x11);
    outb(PIC_SLAVE_COMMAND_PORT, 0x11);

    outb(PIC_MASTER_DATA_PORT, IRQ_OFFSET);
    outb(PIC_SLAVE_DATA_PORT, IRQ_OFFSET + 8);

    outb(PIC_MASTER_DATA_PORT, 0x04);
    outb(PIC_MASTER_DATA_PORT, 0x02);

    outb(PIC_MASTER_DATA_PORT, 0x01);
    outb(PIC_MASTER_DATA_PORT, 0x01);

    outb(PIC_MASTER_DATA_PORT, 0x00);
    outb(PIC_MASTER_DATA_PORT, 0x00);

}

void irq_ack(uint8_t irq) {
    outb(PIC_MASTER_COMMAND_PORT, 0x20);
    if(irq >= 40) outb(PIC_SLAVE_COMMAND_PORT, 0x20);
}