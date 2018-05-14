#ifndef KERNEL_INTERRUPT_H
#define KERNEL_INTERRUPT_H

#include <stdint.h>

#include "common.h"
#include "port.h"

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

#define IRQ_TIMER								0x00
#define IRQ_KEYBOARD							0x01
#define IRQ_CASCADE								0x02
#define IRQ_COM2								0x03
#define IRQ_COM1								0x04
#define IRQ_LPT2								0x05
#define IRQ_FLOPPY								0x06
#define IRQ_LPT1								0x07
#define IRQ_CMOS_REAL_TIME_CLOCK				0x08
#define IRQ_PERIPHERALS_1						0x09
#define IRQ_PERIPHERALS_2						0x0A
#define IRQ_PERIPHERALS_3						0x0B
#define IRQ_MOUSE								0x0C
#define IRQ_COPRPCESSOR							0x0D
#define IRQ_PRIMARY_ATA							0x0E
#define IRQ_SECONDARY_ATA						0x0F

#define IRQ_SYSCALL								0x80

typedef struct cpu_state_t {
	uint32_t ds; /* Data segment selector */
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
	uint32_t int_no, code; /* Interrupt number and code (excpetions - error code, irq - number, syscall - function) */
	uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} PACKED registers_t;

typedef void(*interrupt_handler)(registers_t* regs);

void initialize_interrupts(void);
void register_interrupt_handler(uint8_t interrupt, interrupt_handler handler);

#endif 
