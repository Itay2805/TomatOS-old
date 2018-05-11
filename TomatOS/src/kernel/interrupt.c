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
} PACKED gate_descriptor_t;

typedef struct idt_pointer_t {
	uint16_t size;
	uintptr_t base;
} PACKED idt_pointer_t;


static interrupt_handler handlers[256];
static idt_pointer_t idt_ptr;
static gate_descriptor_t interrupt_descriptor_table[256];

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

// syscall
extern void handle_syscall();

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

void handle_interrupt(registers_t regs) {
	uint8_t* t = (uint8_t*)0xb8000;
	char buf[256];
	itoa(regs.int_no, buf, 10);
	t[0] = buf[0];
	t[1] = 0xF;
	t[2] = buf[1];
	t[3] = 0xF;
	t[4] = buf[2];
	t[5] = 0xF;

	itoa(regs.code, buf, 10);
	t[10] = buf[0];
	t[11] = 0xF;
	t[12] = buf[1];
	t[13] = 0xF;
	t[14] = buf[2];
	t[15] = 0xF;


	if (handlers[regs.int_no] != NULL) {
		handlers[regs.int_no](regs);
	}
	

	// acknowledge irq
	if (IRQ_OFFSET <= regs.int_no && regs.int_no < IRQ_OFFSET + 16) {
		port_write8(PIC_MASTER_COMMAND_PORT, 0x20);
		if (IRQ_OFFSET + 8 <= regs.int_no) {
			port_write8(PIC_SLAVE_COMMAND_PORT, 0x20);
		}
	}
}

static void set_interrupt_descriptor_entry(uint8_t interrupt, uint16_t code_segment, void(*handler)(), uint8_t descriptorPrivLevel, uint8_t descriptorType) {
	interrupt_descriptor_table[interrupt].handler_address_low_bits = ((uint32_t)handler) & 0xFFFF;
	interrupt_descriptor_table[interrupt].gdt_code_segment_selector = code_segment;
	interrupt_descriptor_table[interrupt].reserved = 0;
	interrupt_descriptor_table[interrupt].access = IDT_DESC_PRESENT | ((descriptorPrivLevel & 3) << 5) | descriptorType;
	interrupt_descriptor_table[interrupt].handler_address_high_bits = (((uint32_t)handler) >> 16) & 0xFFFF;
}

void initialize_interrupts(void) {
	uint16_t codeSegment = (uint16_t)CODE_SEGMENT;

	// set default handlers
	for (uint16_t i = 0; i < 256; i++) {
		set_interrupt_descriptor_entry(i, codeSegment, &ignore_interrupt, 0, IDT_INTERRUPT_GATE);
		handlers[i] = NULL;
	}
	
	set_interrupt_descriptor_entry(0x00, codeSegment, &handle_exception_interrupt_00, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x01, codeSegment, &handle_exception_interrupt_01, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x02, codeSegment, &handle_exception_interrupt_02, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x03, codeSegment, &handle_exception_interrupt_03, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x04, codeSegment, &handle_exception_interrupt_04, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x05, codeSegment, &handle_exception_interrupt_05, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x06, codeSegment, &handle_exception_interrupt_06, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x07, codeSegment, &handle_exception_interrupt_07, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x08, codeSegment, &handle_exception_interrupt_08, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x09, codeSegment, &handle_exception_interrupt_09, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0A, codeSegment, &handle_exception_interrupt_0A, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0B, codeSegment, &handle_exception_interrupt_0B, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0C, codeSegment, &handle_exception_interrupt_0C, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0D, codeSegment, &handle_exception_interrupt_0D, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0E, codeSegment, &handle_exception_interrupt_0E, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0F, codeSegment, &handle_exception_interrupt_0F, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x10, codeSegment, &handle_exception_interrupt_10, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x11, codeSegment, &handle_exception_interrupt_11, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x12, codeSegment, &handle_exception_interrupt_12, 0, IDT_INTERRUPT_GATE);
	
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

	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x00, codeSegment, &handle_interrupt_request_00, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x01, codeSegment, &handle_interrupt_request_01, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x02, codeSegment, &handle_interrupt_request_02, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x03, codeSegment, &handle_interrupt_request_03, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x04, codeSegment, &handle_interrupt_request_04, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x05, codeSegment, &handle_interrupt_request_05, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x06, codeSegment, &handle_interrupt_request_06, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x07, codeSegment, &handle_interrupt_request_07, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x08, codeSegment, &handle_interrupt_request_08, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x09, codeSegment, &handle_interrupt_request_09, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x0A, codeSegment, &handle_interrupt_request_0A, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x0B, codeSegment, &handle_interrupt_request_0B, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x0C, codeSegment, &handle_interrupt_request_0C, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x0D, codeSegment, &handle_interrupt_request_0D, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x0E, codeSegment, &handle_interrupt_request_0E, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(IRQ_OFFSET + 0x0F, codeSegment, &handle_interrupt_request_0F, 0, IDT_INTERRUPT_GATE);
	
	set_interrupt_descriptor_entry(IRQ_SYSCALL, codeSegment, handle_syscall, 0, IDT_INTERRUPT_GATE);

	idt_ptr.size = 256 * sizeof(gate_descriptor_t) - 1;
	idt_ptr.base = (uintptr_t)&interrupt_descriptor_table;
	ASM(asm volatile("lidt %0" : : "m"(idt_ptr)));

	ASM(asm("sti"));
}

#undef REQUEST_INTERRUPT_OFFSET
#undef PIC_MASTER_COMMAND_PORT
#undef PIC_MASTER_DATA_PORT
#undef PIC_SLAVE_COMMAND_PORT
#undef PIC_SLAVE_DATA_PORT
#undef IDT_INTERRUPT_GATE
#undef IDT_DESC_PRESENT 
