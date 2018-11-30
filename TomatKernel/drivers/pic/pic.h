#ifndef DRIVERS_PIC_H
#define DRIVERS_PIC_H

#include <stdint.h>

/*

Programmable-Interrupt-Controller

Basic driver for a PIC, might eventually add an option to mask interrupts as well

Should probably also add support for APIC or IOAPIC

*/

#define PIC_MASTER          0x20
#define PIC_MASTER_COMMAND  PIC_MASTER
#define PIC_MASTER_DATA     (PIC_MASTER + 1)

#define PIC_SLAVE           0xA0
#define PIC_SLAVE_COMMAND   PIC_SLAVE
#define PIC_SLAVE_DATA      (PIC_SLAVE + 1)

#define PIC_COMMAND_END_OF_INTERRUPT 0x20

#define ICW1_ICW4	        0x01
#define ICW1_SINGLE	        0x02
#define ICW1_INTERVAL4	    0x04
#define ICW1_LEVEL	        0x08
#define ICW1_INIT	        0x10
 
#define ICW4_8086	        0x01    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	        0x02	/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	    0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	    0x0C	/* Buffered mode/master */
#define ICW4_SFNM	        0x10	/* Special fully nested (not) */

void pic_remap(uint8_t master_offset, uint8_t slave_offset);
void pic_send_eoi(uint8_t irq);

extern void pic_disable();

#endif