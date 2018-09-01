#include "isr.h"

#include <string.h>
#include <stddef.h>

#include <core/term.h>

#include "pic.h"

static irq_handler_t handlers[256];

void isr_init(void) {
	term_write("[isr] Initializing\n");

    memset(handlers, 0, sizeof(handlers));
}

void register_irq_handler(uint8_t num, irq_handler_t handler) {
    handlers[num] = handler;
}

void kernel_exception_handler(registers_t regs) {
	switch (regs.eax) {
		case EXCEPTION_DIVIDE_BY_ZERO:					kpanic("EXCEPTION_DIVIDE_BY_ZERO"); break;
		case EXCEPTION_DEBUG:							kpanic("EXCEPTION_DEBUG"); break;
		case EXCEPTION_NON_MASKABLE_INTERRUPT:			kpanic("EXCEPTION_NON_MASKABLE_INTERRUPT"); break;
		case EXCEPTION_BREAKPOINT:						kpanic("EXCEPTION_BREAKPOINT"); break;
		case EXCEPTION_OVERFLOW:						kpanic("EXCEPTION_OVERFLOW"); break;
		case EXCEPTION_BOUND_RANGE_EXCEEDED:			kpanic("EXCEPTION_BOUND_RANGE_EXCEEDED"); break;
		case EXCEPTION_INVALID_OPCODE:					kpanic("EXCEPTION_INVALID_OPCODE"); break;
		case EXCEPTION_DEVICE_NOT_AVAILABLE:			kpanic("EXCEPTION_DEVICE_NOT_AVAILABLE"); break;
		case EXCEPTION_DOUBLE_FAULT:					kpanic("EXCEPTION_DOUBLE_FAULT"); break;
		case EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN:		kpanic("EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN"); break;
		case EXCEPTION_INVALID_TSS:						kpanic("EXCEPTION_INVALID_TSS"); break;
		case EXCEPTION_SEGMENT_NOT_PRESENT:				kpanic("EXCEPTION_SEGMENT_NOT_PRESENT"); break;
		case EXCEPTION_STACK_SEGMENT_FAULT:				kpanic("EXCEPTION_STACK_SEGMENT_FAULT"); break;
		case EXCEPTION_GENERAL_PROTECTION_FAULT:		kpanic("EXCEPTION_GENERAL_PROTECTION_FAULT"); break;
		case EXCEPTION_PAGE_FAULT:						kpanic("EXCEPTION_PAGE_FAULT"); break;
		case EXCEPTION_0xF:								kpanic("EXCEPTION_0xF"); break;
		case EXCEPTION_X87_FLOATING_POINT_EXCEPTION:	kpanic("EXCEPTION_X87_FLOATING_POINT_EXCEPTION"); break;
		case EXCEPTION_ALIGNMENT_CHECK:					kpanic("EXCEPTION_ALIGNMENT_CHECK"); break;
		case EXCEPTION_MACHINE_CHECK:					kpanic("EXCEPTION_MACHINE_CHECK"); break;
		case EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION:	kpanic("EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION"); break;
		case EXCEPTION_VIRTUALIZATION_EXCEPTION:		kpanic("EXCEPTION_VIRTUALIZATION_EXCEPTION"); break;
		case EXCEPTION_0x15:							kpanic("EXCEPTION_0x15"); break;
		case EXCEPTION_0x16:							kpanic("EXCEPTION_0x16"); break;
		case EXCEPTION_0x17:							kpanic("EXCEPTION_0x17"); break;
		case EXCEPTION_0x18:							kpanic("EXCEPTION_0x18"); break;
		case EXCEPTION_0x19:							kpanic("EXCEPTION_0x19"); break;
		case EXCEPTION_0x1A:							kpanic("EXCEPTION_0x1A"); break;
		case EXCEPTION_0x1B:							kpanic("EXCEPTION_0x1B"); break;
		case EXCEPTION_0x1C:							kpanic("EXCEPTION_0x1C"); break;
		case EXCEPTION_0x1D:							kpanic("EXCEPTION_0x1D"); break;
		case EXCEPTION_SECURITY_EXCEPTION:				kpanic("EXCEPTION_SECURITY_EXCEPTION"); break;
		case EXCEPTION_0x1F:							kpanic("EXCEPTION_0x1F"); break;
	}
}

void kernel_irq_handler(registers_t regs) {
    if(handlers[regs.int_no] != NULL) {
        handlers[regs.int_no](&regs);
    }
    
    irq_ack(regs.int_no);
}
