#include "isr.h"
#include "idt.h"

#include <api/term.h>
#include <api/os.h>

char *exception_messages[32];

void kernel_isr_install() {
	kernel_set_idt_gate(0, (uint32_t)isr0);
	kernel_set_idt_gate(1, (uint32_t)isr1);
	kernel_set_idt_gate(2, (uint32_t)isr2);
	kernel_set_idt_gate(3, (uint32_t)isr3);
	kernel_set_idt_gate(4, (uint32_t)isr4);
	kernel_set_idt_gate(5, (uint32_t)isr5);
	kernel_set_idt_gate(6, (uint32_t)isr6);
	kernel_set_idt_gate(7, (uint32_t)isr7);
	kernel_set_idt_gate(8, (uint32_t)isr8);
	kernel_set_idt_gate(9, (uint32_t)isr9);
	kernel_set_idt_gate(10, (uint32_t)isr10);
	kernel_set_idt_gate(11, (uint32_t)isr11);
	kernel_set_idt_gate(12, (uint32_t)isr12);
	kernel_set_idt_gate(13, (uint32_t)isr13);
	kernel_set_idt_gate(14, (uint32_t)isr14);
	kernel_set_idt_gate(15, (uint32_t)isr15);
	kernel_set_idt_gate(16, (uint32_t)isr16);
	kernel_set_idt_gate(17, (uint32_t)isr17);
	kernel_set_idt_gate(18, (uint32_t)isr18);
	kernel_set_idt_gate(19, (uint32_t)isr19);
	kernel_set_idt_gate(20, (uint32_t)isr20);
	kernel_set_idt_gate(21, (uint32_t)isr21);
	kernel_set_idt_gate(22, (uint32_t)isr22);
	kernel_set_idt_gate(23, (uint32_t)isr23);
	kernel_set_idt_gate(24, (uint32_t)isr24);
	kernel_set_idt_gate(25, (uint32_t)isr25);
	kernel_set_idt_gate(26, (uint32_t)isr26);
	kernel_set_idt_gate(27, (uint32_t)isr27);
	kernel_set_idt_gate(28, (uint32_t)isr28);
	kernel_set_idt_gate(29, (uint32_t)isr29);
	kernel_set_idt_gate(30, (uint32_t)isr30);
	kernel_set_idt_gate(31, (uint32_t)isr31);

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