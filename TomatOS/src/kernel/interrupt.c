#include "interrupt.h"

#include <stddef.h>

#include "gdt.h"

#define IRQ_OFFSET 32

#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_COMMAND_PORT 0xA0
#define PIC_SLAVE_DATA_PORT 0xA1

#define IDT_INTERRUPT_GATE 0xE
#define IDT_DESC_PRESENT 0x80

typedef struct gate_descriptor_t {
	uint16_t handler_address_low_bits;
	uint16_t gdt_code_segment_selector;
	uint8_t reserved;
	uint8_t access;
	uint16_t handler_address_high_bits;
} PACKED idt_gate_t;

typedef struct idt_pointer_t {
	uint16_t size;
	uintptr_t base;
} PACKED idt_register_t;


static interrupt_handler handlers[256];
static idt_register_t idt_reg;
static idt_gate_t idt[256];

// default interrupt
extern void ignore_interrupt();

// request interrupts
extern void handle_interrupt_request_00();
extern void handle_interrupt_request_01();
extern void handle_interrupt_request_02();
extern void handle_interrupt_request_03();
extern void handle_interrupt_request_04();
extern void handle_interrupt_request_05();
extern void handle_interrupt_request_06();
extern void handle_interrupt_request_07();
extern void handle_interrupt_request_08();
extern void handle_interrupt_request_09();
extern void handle_interrupt_request_0A();
extern void handle_interrupt_request_0B();
extern void handle_interrupt_request_0C();
extern void handle_interrupt_request_0D();
extern void handle_interrupt_request_0E();
extern void handle_interrupt_request_0F();

// exceptions
extern void handle_exception_interrupt_00();
extern void handle_exception_interrupt_01();
extern void handle_exception_interrupt_02();
extern void handle_exception_interrupt_03();
extern void handle_exception_interrupt_04();
extern void handle_exception_interrupt_05();
extern void handle_exception_interrupt_06();
extern void handle_exception_interrupt_07();
extern void handle_exception_interrupt_08();
extern void handle_exception_interrupt_09();
extern void handle_exception_interrupt_0A();
extern void handle_exception_interrupt_0B();
extern void handle_exception_interrupt_0C();
extern void handle_exception_interrupt_0D();
extern void handle_exception_interrupt_0E();
extern void handle_exception_interrupt_0F();
extern void handle_exception_interrupt_10();
extern void handle_exception_interrupt_11();
extern void handle_exception_interrupt_12();

void register_interrupt_handler(uint8_t interrupt, interrupt_handler handler) {
	handlers[interrupt] = handler;
}

#include <string.h>

void kernel_irq_handler(registers_t r) {
	if (r.int_no >= 40) port_write8(PIC_SLAVE_COMMAND_PORT, 0x20);
	port_write8(PIC_MASTER_COMMAND_PORT, 0x20);

	if (handlers[r.int_no] != NULL) {
		handlers[r.int_no](r);
	}
}

void kernel_exception_handler(registers_t r) {
	// @TODO: handling exceptions
}

static void set_idt_gate(uint8_t n, void(*handler)(void)) {
	idt[n].handler_address_low_bits = (uint16_t)(((uint32_t)handler) & 0xFFFF);
	idt[n].gdt_code_segment_selector = CODE_SEGMENT;
	idt[n].reserved = 0;
	idt[n].access = 0x8E;
	idt[n].handler_address_high_bits = (uint16_t)(((uint32_t)handler >> 16) & 0xFFFF);
}

void initialize_interrupts(void) {
	// set default handlers
	for (uint16_t i = 0; i < 256; i++) {
		set_idt_gate(i, &ignore_interrupt);
		handlers[i] = NULL;
	}
	
	set_idt_gate(0x00, &handle_exception_interrupt_00);
	set_idt_gate(0x01, &handle_exception_interrupt_01);
	set_idt_gate(0x02, &handle_exception_interrupt_02);
	set_idt_gate(0x03, &handle_exception_interrupt_03);
	set_idt_gate(0x04, &handle_exception_interrupt_04);
	set_idt_gate(0x05, &handle_exception_interrupt_05);
	set_idt_gate(0x06, &handle_exception_interrupt_06);
	set_idt_gate(0x07, &handle_exception_interrupt_07);
	set_idt_gate(0x08, &handle_exception_interrupt_08);
	set_idt_gate(0x09, &handle_exception_interrupt_09);
	set_idt_gate(0x0A, &handle_exception_interrupt_0A);
	set_idt_gate(0x0B, &handle_exception_interrupt_0B);
	set_idt_gate(0x0C, &handle_exception_interrupt_0C);
	set_idt_gate(0x0D, &handle_exception_interrupt_0D);
	set_idt_gate(0x0E, &handle_exception_interrupt_0E);
	set_idt_gate(0x0F, &handle_exception_interrupt_0F);
	set_idt_gate(0x10, &handle_exception_interrupt_10);
	set_idt_gate(0x11, &handle_exception_interrupt_11);
	set_idt_gate(0x12, &handle_exception_interrupt_12);
	
	port_write8(PIC_MASTER_COMMAND_PORT, 0x11);
	port_write8(PIC_SLAVE_COMMAND_PORT, 0x11);
	
	// remapping 
	port_write8(PIC_MASTER_DATA_PORT, IRQ_OFFSET);
	port_write8(PIC_SLAVE_DATA_PORT, IRQ_OFFSET + 8);

	port_write8(PIC_MASTER_DATA_PORT, 0x04);
	port_write8(PIC_SLAVE_DATA_PORT, 0x02);

	port_write8(PIC_MASTER_DATA_PORT, 0x01);
	port_write8(PIC_SLAVE_DATA_PORT, 0x01);

	port_write8(PIC_MASTER_DATA_PORT, 0x00);
	port_write8(PIC_SLAVE_DATA_PORT, 0x00);

	set_idt_gate(IRQ_OFFSET + 0x00, &handle_interrupt_request_00);
	set_idt_gate(IRQ_OFFSET + 0x01, &handle_interrupt_request_01);
	set_idt_gate(IRQ_OFFSET + 0x02, &handle_interrupt_request_02);
	set_idt_gate(IRQ_OFFSET + 0x03, &handle_interrupt_request_03);
	set_idt_gate(IRQ_OFFSET + 0x04, &handle_interrupt_request_04);
	set_idt_gate(IRQ_OFFSET + 0x05, &handle_interrupt_request_05);
	set_idt_gate(IRQ_OFFSET + 0x06, &handle_interrupt_request_06);
	set_idt_gate(IRQ_OFFSET + 0x07, &handle_interrupt_request_07);
	set_idt_gate(IRQ_OFFSET + 0x08, &handle_interrupt_request_08);
	set_idt_gate(IRQ_OFFSET + 0x09, &handle_interrupt_request_09);
	set_idt_gate(IRQ_OFFSET + 0x0A, &handle_interrupt_request_0A);
	set_idt_gate(IRQ_OFFSET + 0x0B, &handle_interrupt_request_0B);
	set_idt_gate(IRQ_OFFSET + 0x0C, &handle_interrupt_request_0C);
	set_idt_gate(IRQ_OFFSET + 0x0D, &handle_interrupt_request_0D);
	set_idt_gate(IRQ_OFFSET + 0x0E, &handle_interrupt_request_0E);
	set_idt_gate(IRQ_OFFSET + 0x0F, &handle_interrupt_request_0F);
	
	idt_reg.base = (uintptr_t)&idt;
	idt_reg.size = 256 * sizeof(idt_gate_t) - 1;
	ASM(asm volatile("lidtl (%0)" : : "r"(&idt_reg)));

	ASM(asm("sti"));
}

#undef REQUEST_INTERRUPT_OFFSET
#undef PIC_MASTER_COMMAND_PORT
#undef PIC_MASTER_DATA_PORT
#undef PIC_SLAVE_COMMAND_PORT
#undef PIC_SLAVE_DATA_PORT
#undef IDT_INTERRUPT_GATE
#undef IDT_DESC_PRESENT 
