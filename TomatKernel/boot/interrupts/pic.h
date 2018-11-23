#ifndef BOOT_PIC_H
#define BOOT_PIC_H

#include <stdint.h>

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_COMMAND_PORT 0xA0
#define PIC_SLAVE_DATA_PORT 0xA1

#define IRQ_OFFSET 32

#define EXCEPTION_DIVIDE_BY_ZERO					0x00
#define EXCEPTION_DEBUG								0x01
#define EXCEPTION_NON_MASKABLE_INTERRUPT			0x02
#define EXCEPTION_BREAKPOINT						0x03
#define EXCEPTION_OVERFLOW							0x04
#define EXCEPTION_BOUND_RANGE_EXCEEDED				0x05
#define EXCEPTION_INVALID_OPCODE					0x06
#define EXCEPTION_DEVICE_NOT_AVAILABLE				0x07
#define EXCEPTION_DOUBLE_FAULT						0x08
#define EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN		0x09
#define EXCEPTION_INVALID_TSS						0x0A
#define EXCEPTION_SEGMENT_NOT_PRESENT				0x0B
#define EXCEPTION_STACK_SEGMENT_FAULT				0x0C
#define EXCEPTION_GENERAL_PROTECTION_FAULT			0x0D
#define EXCEPTION_PAGE_FAULT						0x0E
#define EXCEPTION_0xF								0x0F
#define EXCEPTION_X87_FLOATING_POINT_EXCEPTION		0x10
#define EXCEPTION_ALIGNMENT_CHECK					0x11
#define EXCEPTION_MACHINE_CHECK						0x12
#define EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION		0x13
#define EXCEPTION_VIRTUALIZATION_EXCEPTION			0x14
#define EXCEPTION_0x15								0x15
#define EXCEPTION_0x16								0x16
#define EXCEPTION_0x17								0x17
#define EXCEPTION_0x18								0x18
#define EXCEPTION_0x19								0x19
#define EXCEPTION_0x1A								0x1A
#define EXCEPTION_0x1B								0x1B
#define EXCEPTION_0x1C								0x1C
#define EXCEPTION_0x1D								0x1D
#define EXCEPTION_SECURITY_EXCEPTION				0x1E
#define EXCEPTION_0x1F								0x1F

// we can't actually check for this exception on kernel level
#define EXCEPTION_TRIPLE_FAULT

#define IRQ_TIMER								(IRQ_OFFSET + 0x00)
#define IRQ_KEYBOARD							(IRQ_OFFSET + 0x01)
#define IRQ_CASCADE								(IRQ_OFFSET + 0x02)
#define IRQ_COM2								(IRQ_OFFSET + 0x03)
#define IRQ_COM1								(IRQ_OFFSET + 0x04)
#define IRQ_LPT2								(IRQ_OFFSET + 0x05)
#define IRQ_FLOPPY								(IRQ_OFFSET + 0x06)
#define IRQ_LPT1								(IRQ_OFFSET + 0x07)
#define IRQ_CMOS_REAL_TIME_CLOCK				(IRQ_OFFSET + 0x08)
#define IRQ_PERIPHERALS_1						(IRQ_OFFSET + 0x09)
#define IRQ_PERIPHERALS_2						(IRQ_OFFSET + 0x0A)
#define IRQ_PERIPHERALS_3						(IRQ_OFFSET + 0x0B)
#define IRQ_MOUSE								(IRQ_OFFSET + 0x0C)
#define IRQ_COPRPCESSOR							(IRQ_OFFSET + 0x0D)
#define IRQ_PRIMARY_ATA							(IRQ_OFFSET + 0x0E)
#define IRQ_SECONDARY_ATA						(IRQ_OFFSET + 0x0F)

#define IRQ_SYSCALL                             0x80

#ifdef __cplusplus
extern "C" {
#endif

void pic_init();
void irq_ack(uint8_t irq);

#ifdef __cplusplus
}
#endif

#endif