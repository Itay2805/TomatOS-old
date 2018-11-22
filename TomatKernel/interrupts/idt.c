#include "idt.h"

#include <gdt/gdt.h>
#include <drivers/pic/pic.h>

//////////////////////////////////
// Interrupt handle stubs
//////////////////////////////////

extern void handle_exception_divide_by_zero();
extern void handle_exception_debug();
extern void handle_exception_non_maskable_interrupt();
extern void handle_exception_breakpoint();
extern void handle_exception_overflow();
extern void handle_exception_bound_range_exceeded();
extern void handle_exception_invalid_opcode();
extern void handle_exception_device_not_available();
extern void handle_exception_double_fault();
extern void handle_exception_coprocessor_segment_overrun();
extern void handle_exception_invalid_tss();
extern void handle_exception_segment_not_present();
extern void handle_exception_stack_segment_fault();
extern void handle_exception_general_protection_fault();
extern void handle_exception_page_fault();
extern void handle_exception_0xf();
extern void handle_exception_x87_floating_point_exception();
extern void handle_exception_alignment_check();
extern void handle_exception_machine_check();
extern void handle_exception_simd_floating_point_exception();
extern void handle_exception_virtualization_exception();
extern void handle_exception_0x15();
extern void handle_exception_0x16();
extern void handle_exception_0x17();
extern void handle_exception_0x18();
extern void handle_exception_0x19();
extern void handle_exception_0x1a();
extern void handle_exception_0x1b();
extern void handle_exception_0x1c();
extern void handle_exception_0x1d();
extern void handle_exception_security_exception();
extern void handle_exception_0x1f();

extern void handle_irq_timer();
extern void handle_irq_keyboard();
extern void handle_irq_cascade();
extern void handle_irq_com2();
extern void handle_irq_com1();
extern void handle_irq_lpt2();
extern void handle_irq_floppy();
extern void handle_irq_lpt1();
extern void handle_irq_cmos_real_time_clock();
extern void handle_irq_peripherals_1();
extern void handle_irq_peripherals_2();
extern void handle_irq_peripherals_3();
extern void handle_irq_mouse();
extern void handle_irq_coprpcessor();
extern void handle_irq_primary_ata();
extern void handle_irq_secondary_ata();

extern void handle_irq_syscall();

//////////////////////////////////
// IDT
//////////////////////////////////

static idt_gate_t gates[256];
idt_t idt = { sizeof(gates), gates };

irq_handler_t irq_handlers[256];

typedef void(*idt_gate_handler_t)();

static void set_entry(int index, idt_gate_handler_t base, uint8_t ring) {
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

//////////////////////////////////
// Interrupt handlers
//////////////////////////////////

void kernel_exception_handler(registers_t* regs) {

}

void kernel_irq_handler(registers_t* regs) {
}

//////////////////////////////////
// IDT initialization
//////////////////////////////////

void idt_init() {

    set_entry(0x00);

    // actuall load the idt
	asm volatile("lidtl (%0)" : : "r"(&idt));
    asm("sti");

}

