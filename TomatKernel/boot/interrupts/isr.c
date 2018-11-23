#include "isr.h"

#include <string.h>
#include <stddef.h>

#include <core/graphics/term.h>
#include <core/process/process.h>

#include "pic.h"

static irq_handler_t handlers[256];

void isr_init(void) {
    memset(handlers, 0, sizeof(handlers));
}

void register_irq_handler(uint8_t num, irq_handler_t handler) {
    handlers[num] = handler;
}

void kernel_exception_handler(registers_t regs) {
	term_clear();
	term_set_cursor_pos(0, 0);
	term_set_background_color(COLOR_RED);
	term_set_text_color(COLOR_WHITE);
	term_write("Exception: ");

	switch (regs.int_no) {
		case EXCEPTION_DIVIDE_BY_ZERO:					term_write("EXCEPTION_DIVIDE_BY_ZERO"); break;
		case EXCEPTION_DEBUG:							term_write("EXCEPTION_DEBUG"); break;
		case EXCEPTION_NON_MASKABLE_INTERRUPT:			term_write("EXCEPTION_NON_MASKABLE_INTERRUPT"); break;
		case EXCEPTION_BREAKPOINT:						term_write("EXCEPTION_BREAKPOINT"); break;
		case EXCEPTION_OVERFLOW:						term_write("EXCEPTION_OVERFLOW"); break;
		case EXCEPTION_BOUND_RANGE_EXCEEDED:			term_write("EXCEPTION_BOUND_RANGE_EXCEEDED"); break;
		case EXCEPTION_INVALID_OPCODE:					term_write("EXCEPTION_INVALID_OPCODE"); break;
		case EXCEPTION_DEVICE_NOT_AVAILABLE:			term_write("EXCEPTION_DEVICE_NOT_AVAILABLE"); break;
		case EXCEPTION_DOUBLE_FAULT:					term_write("EXCEPTION_DOUBLE_FAULT"); break;
		case EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN:		term_write("EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN"); break;
		case EXCEPTION_INVALID_TSS:						term_write("EXCEPTION_INVALID_TSS"); break;
		case EXCEPTION_SEGMENT_NOT_PRESENT:				term_write("EXCEPTION_SEGMENT_NOT_PRESENT"); break;
		case EXCEPTION_STACK_SEGMENT_FAULT:				term_write("EXCEPTION_STACK_SEGMENT_FAULT"); break;
		case EXCEPTION_GENERAL_PROTECTION_FAULT:		term_write("EXCEPTION_GENERAL_PROTECTION_FAULT"); break;
		case EXCEPTION_PAGE_FAULT:						term_write("EXCEPTION_PAGE_FAULT"); break;
		case EXCEPTION_0xF:								term_write("EXCEPTION_0xF"); break;
		case EXCEPTION_X87_FLOATING_POINT_EXCEPTION:	term_write("EXCEPTION_X87_FLOATING_POINT_EXCEPTION"); break;
		case EXCEPTION_ALIGNMENT_CHECK:					term_write("EXCEPTION_ALIGNMENT_CHECK"); break;
		case EXCEPTION_MACHINE_CHECK:					term_write("EXCEPTION_MACHINE_CHECK"); break;
		case EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION:	term_write("EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION"); break;
		case EXCEPTION_VIRTUALIZATION_EXCEPTION:		term_write("EXCEPTION_VIRTUALIZATION_EXCEPTION"); break;
		case EXCEPTION_0x15:							term_write("EXCEPTION_0x15"); break;
		case EXCEPTION_0x16:							term_write("EXCEPTION_0x16"); break;
		case EXCEPTION_0x17:							term_write("EXCEPTION_0x17"); break;
		case EXCEPTION_0x18:							term_write("EXCEPTION_0x18"); break;
		case EXCEPTION_0x19:							term_write("EXCEPTION_0x19"); break;
		case EXCEPTION_0x1A:							term_write("EXCEPTION_0x1A"); break;
		case EXCEPTION_0x1B:							term_write("EXCEPTION_0x1B"); break;
		case EXCEPTION_0x1C:							term_write("EXCEPTION_0x1C"); break;
		case EXCEPTION_0x1D:							term_write("EXCEPTION_0x1D"); break;
		case EXCEPTION_SECURITY_EXCEPTION:				term_write("EXCEPTION_SECURITY_EXCEPTION"); break;
		case EXCEPTION_0x1F:							term_write("EXCEPTION_0x1F"); break;
	}

	term_write("\n");

	char numBuf[20];

	switch (regs.int_no) {
		case EXCEPTION_DOUBLE_FAULT:
		case EXCEPTION_INVALID_TSS:
		case EXCEPTION_SEGMENT_NOT_PRESENT:
		case EXCEPTION_STACK_SEGMENT_FAULT:
		case EXCEPTION_GENERAL_PROTECTION_FAULT:
		case EXCEPTION_PAGE_FAULT:
			term_write("ERROR CODE: 0x");
			if (regs.code != 0) {
				uitoa(regs.code, numBuf, 16);
				term_write(numBuf);
			}
			else {
				term_write("0");
			}
			term_write("\n");
	}

	term_write("\n");

	term_set_background_color(COLOR_BLACK);
	process_t* process = process_get_running();

	term_write("Process:\n");

	term_write("\tUID: ");
	if (process == 0) {
		term_write("NO RUNNING PROCESS");
	}
	else {
		uitoa(process->uid, numBuf, 16);
		term_write(numBuf);
	}
	term_write("\n");

	term_write("\tUser: ");
	if (process == 0) {
		term_write("NO RUNNING PROCESS");
	}
	else {
		uitoa(process->user, numBuf, 16);
		term_write(numBuf);
	}
	term_write("\n");

	term_write("\n");

	term_write("Registers: \n");

	uitoa(regs.eip, numBuf, 16);
	term_write("EIP: ");
	term_write(numBuf);
	term_write("\n");

	// TODO: in the future this will need to just terminate the process
	// and maybe save this information on to the disk or idk
	// process_kill(&regs, process->uid);

	// stop execution...
	while (1) {
		asm("nop");
		asm("hlt");
	}
}

void kernel_irq_handler(registers_t regs) {
    if(handlers[regs.int_no] != NULL) {
        handlers[regs.int_no](&regs);
    }
    
    irq_ack(regs.int_no);
}
