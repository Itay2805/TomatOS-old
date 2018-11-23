#include "idt.h"

#include <string.h>

#include "pic.h"
#include "isr.h"
#include "../gdt/gdt.h"

idt_t idt;
static idt_gate_t gates[256];

void idt_init(void) {
    memset(gates, 0, sizeof(gates));
    idt.size = sizeof(gates) - 1;
    idt.gates = gates;
    pic_init();
    
    idt_set_entry(0x00, handle_exception_interrupt_00, 0);
    idt_set_entry(0x01, handle_exception_interrupt_01, 3);  // DEBUG. allow user
    idt_set_entry(0x02, handle_exception_interrupt_02, 0);
    idt_set_entry(0x03, handle_exception_interrupt_03, 3);  // BREAKPOINT. allow user
    idt_set_entry(0x04, handle_exception_interrupt_04, 0);
    idt_set_entry(0x05, handle_exception_interrupt_05, 0);
    idt_set_entry(0x06, handle_exception_interrupt_06, 0);
    idt_set_entry(0x07, handle_exception_interrupt_07, 0);
    idt_set_entry(0x08, handle_exception_interrupt_08, 0);
    idt_set_entry(0x09, handle_exception_interrupt_09, 0);
    idt_set_entry(0x0A, handle_exception_interrupt_0A, 0);
    idt_set_entry(0x0B, handle_exception_interrupt_0B, 0);
    idt_set_entry(0x0C, handle_exception_interrupt_0C, 0);
    idt_set_entry(0x0D, handle_exception_interrupt_0D, 0);
    idt_set_entry(0x0E, handle_exception_interrupt_0E, 0);
    idt_set_entry(0x0F, handle_exception_interrupt_0F, 0);
    idt_set_entry(0x10, handle_exception_interrupt_10, 0);
    idt_set_entry(0x11, handle_exception_interrupt_11, 0);
    idt_set_entry(0x12, handle_exception_interrupt_12, 0);
    idt_set_entry(0x13, handle_exception_interrupt_13, 0);
    idt_set_entry(0x14, handle_exception_interrupt_14, 0);
    idt_set_entry(0x15, handle_exception_interrupt_15, 0);
    idt_set_entry(0x16, handle_exception_interrupt_16, 0);
    idt_set_entry(0x17, handle_exception_interrupt_17, 0);
    idt_set_entry(0x18, handle_exception_interrupt_18, 0);
    idt_set_entry(0x19, handle_exception_interrupt_19, 0);
    idt_set_entry(0x1A, handle_exception_interrupt_1A, 0);
    idt_set_entry(0x1B, handle_exception_interrupt_1B, 0);
    idt_set_entry(0x1C, handle_exception_interrupt_1C, 0);
    idt_set_entry(0x1D, handle_exception_interrupt_1D, 0);
    idt_set_entry(0x1E, handle_exception_interrupt_1E, 0);
    idt_set_entry(0x1F, handle_exception_interrupt_1F, 0);

    idt_set_entry(0x00 + IRQ_OFFSET, handle_interrupt_request_00, 0);
    idt_set_entry(0x01 + IRQ_OFFSET, handle_interrupt_request_01, 0);
    idt_set_entry(0x02 + IRQ_OFFSET, handle_interrupt_request_02, 0);
    idt_set_entry(0x03 + IRQ_OFFSET, handle_interrupt_request_03, 0);
    idt_set_entry(0x04 + IRQ_OFFSET, handle_interrupt_request_04, 0);
    idt_set_entry(0x05 + IRQ_OFFSET, handle_interrupt_request_05, 0);
    idt_set_entry(0x06 + IRQ_OFFSET, handle_interrupt_request_06, 0);
    idt_set_entry(0x07 + IRQ_OFFSET, handle_interrupt_request_07, 0);
    idt_set_entry(0x08 + IRQ_OFFSET, handle_interrupt_request_08, 0);
    idt_set_entry(0x09 + IRQ_OFFSET, handle_interrupt_request_09, 0);
    idt_set_entry(0x0A + IRQ_OFFSET, handle_interrupt_request_0A, 0);
    idt_set_entry(0x0B + IRQ_OFFSET, handle_interrupt_request_0B, 0);
    idt_set_entry(0x0C + IRQ_OFFSET, handle_interrupt_request_0C, 0);
    idt_set_entry(0x0D + IRQ_OFFSET, handle_interrupt_request_0D, 0);
    idt_set_entry(0x0E + IRQ_OFFSET, handle_interrupt_request_0E, 0);
    idt_set_entry(0x0F + IRQ_OFFSET, handle_interrupt_request_0F, 0);

    idt_set_entry(IRQ_SYSCALL, handle_syscall, 3);

	asm volatile("lidtl (%0)" : : "r"(&idt));
	asm("sti");

	isr_init();
}

void idt_set_entry(int index, idt_handle_t base, uint8_t ring) {
    idt_gate_t* gate = &gates[index];
    gate->handler_address_low = (uint32_t)base & 0xFFFF;
    gate->handler_address_high = ((uint32_t)base >> 16) & 0xFFFF;
    gate->present = 1;
    gate->DPL = ring;
    gate->storage_segment = 0;
    gate->gate_type = IDT_INTERRUPT_GATE;
    gate->unused = 0;
    gate->selector = GDT_SEGMENT_CODE;
}
