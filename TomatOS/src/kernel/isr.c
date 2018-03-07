#include "isr.h"
#include "idt.h"

#include <api/term.h>
#include <api/os.h>

#include <memory.h>

#include "../drivers/ports.h"

char *exception_messages[32];

isr_handle_t __interrupt_handlers[256];

void kernel_isr_install() {
	kernel_set_idt_gate(0, (uintptr_t)isr0);
	kernel_set_idt_gate(1, (uintptr_t)isr1);
	kernel_set_idt_gate(2, (uintptr_t)isr2);
	kernel_set_idt_gate(3, (uintptr_t)isr3);
	kernel_set_idt_gate(4, (uintptr_t)isr4);
	kernel_set_idt_gate(5, (uintptr_t)isr5);
	kernel_set_idt_gate(6, (uintptr_t)isr6);
	kernel_set_idt_gate(7, (uintptr_t)isr7);
	kernel_set_idt_gate(8, (uintptr_t)isr8);
	kernel_set_idt_gate(9, (uintptr_t)isr9);
	kernel_set_idt_gate(10, (uintptr_t)isr10);
	kernel_set_idt_gate(11, (uintptr_t)isr11);
	kernel_set_idt_gate(12, (uintptr_t)isr12);
	kernel_set_idt_gate(13, (uintptr_t)isr13);
	kernel_set_idt_gate(14, (uintptr_t)isr14);
	kernel_set_idt_gate(15, (uintptr_t)isr15);
	kernel_set_idt_gate(16, (uintptr_t)isr16);
	kernel_set_idt_gate(17, (uintptr_t)isr17);
	kernel_set_idt_gate(18, (uintptr_t)isr18);
	kernel_set_idt_gate(19, (uintptr_t)isr19);
	kernel_set_idt_gate(20, (uintptr_t)isr20);
	kernel_set_idt_gate(21, (uintptr_t)isr21);
	kernel_set_idt_gate(22, (uintptr_t)isr22);
	kernel_set_idt_gate(23, (uintptr_t)isr23);
	kernel_set_idt_gate(24, (uintptr_t)isr24);
	kernel_set_idt_gate(25, (uintptr_t)isr25);
	kernel_set_idt_gate(26, (uintptr_t)isr26);
	kernel_set_idt_gate(27, (uintptr_t)isr27);
	kernel_set_idt_gate(28, (uintptr_t)isr28);
	kernel_set_idt_gate(29, (uintptr_t)isr29);
	kernel_set_idt_gate(30, (uintptr_t)isr30);
	kernel_set_idt_gate(31, (uintptr_t)isr31);

	port_byte_out(0x20, 0x11);
	port_byte_out(0xA0, 0x11);
	port_byte_out(0x21, 0x20);
	port_byte_out(0xA1, 0x28);
	port_byte_out(0x21, 0x04);
	port_byte_out(0xA1, 0x02);
	port_byte_out(0x21, 0x01);
	port_byte_out(0xA1, 0x01);
	port_byte_out(0x21, 0x0);
	port_byte_out(0xA1, 0x0);

	kernel_set_idt_gate(32, (uintptr_t)irq0);
	kernel_set_idt_gate(33, (uintptr_t)irq1);
	kernel_set_idt_gate(34, (uintptr_t)irq2);
	kernel_set_idt_gate(35, (uintptr_t)irq3);
	kernel_set_idt_gate(36, (uintptr_t)irq4);
	kernel_set_idt_gate(37, (uintptr_t)irq5);
	kernel_set_idt_gate(38, (uintptr_t)irq6);
	kernel_set_idt_gate(39, (uintptr_t)irq7);
	kernel_set_idt_gate(40, (uintptr_t)irq8);
	kernel_set_idt_gate(41, (uintptr_t)irq9);
	kernel_set_idt_gate(42, (uintptr_t)irq10);
	kernel_set_idt_gate(43, (uintptr_t)irq11);
	kernel_set_idt_gate(44, (uintptr_t)irq12);
	kernel_set_idt_gate(45, (uintptr_t)irq13);
	kernel_set_idt_gate(46, (uintptr_t)irq14);
	kernel_set_idt_gate(47, (uintptr_t)irq15);

	kernel_set_idt();

	exception_messages[0] = "Division By Zero";
	exception_messages[1] = "Debug";
	exception_messages[2] = "Non Maskable Interrupt";
	exception_messages[3] = "Breakpoint";
	exception_messages[4] = "Into Detected Overflow";
	exception_messages[5] = "Out of Bounds";
	exception_messages[6] = "Invalid Opcode";
	exception_messages[7] = "No Coprocessor";
	exception_messages[8] = "Double Fault";
	exception_messages[9] = "Coprocessor Segment Overrun";
	exception_messages[10] = "Bad TSS";
	exception_messages[11] = "Segment Not Present";
	exception_messages[12] = "Stack Fault";
	exception_messages[13] = "General Protection Fault";
	exception_messages[14] = "Page Fault";
	exception_messages[15] = "Unknown Interrupt";
	exception_messages[16] = "Coprocessor Fault";
	exception_messages[17] = "Alignment Check";
	exception_messages[18] = "Machine Check";
	exception_messages[19] = "Reserved";
	exception_messages[20] = "Reserved";
	exception_messages[21] = "Reserved";
	exception_messages[22] = "Reserved";
	exception_messages[23] = "Reserved";
	exception_messages[24] = "Reserved";
	exception_messages[25] = "Reserved";
	exception_messages[26] = "Reserved";
	exception_messages[27] = "Reserved";
	exception_messages[28] = "Reserved";
	exception_messages[29] = "Reserved";
	exception_messages[30] = "Reserved";
	exception_messages[31] = "Reserved";
}


void kernel_isr_handler(registers_t r) {
	// lets create an exception event
	event_t event;
	exception_t* exception = (exception_t*)&event;
	exception->type = EVENT_EXCEPTION;
	exception->code = r.int_no;
	exception->msg = exception_messages[r.int_no];
	os_queue_event(event);
}

void kernel_irq_handler(registers_t r) {
	if (r.int_no >= 40) port_byte_out(0xA0, 0x20);
	port_byte_out(0x20, 0x20);

	if (__interrupt_handlers[r.int_no] != nullptr) {
		isr_handle_t handler = __interrupt_handlers[r.int_no];
		handler(r);
	}
}