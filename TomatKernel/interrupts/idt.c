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

extern void handle_syscall();

extern void ignore_interrupt();

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
    // if a process crashes leave a log file and show an error
    // Maybe also attempt to get debug details if available for process
}

void kernel_irq_handler(registers_t* regs) {
    if(irq_handlers[regs->int_no]) {
        irq_handlers[regs->int_no](regs);
    }
    pic_send_eoi(regs->int_no);
}

//////////////////////////////////
// IDT initialization
//////////////////////////////////

void idt_init() {
    set_entry(EXCEPTION_DIVIDE_BY_ZERO, handle_exception_divide_by_zero, 0);
    set_entry(EXCEPTION_DEBUG, handle_exception_debug, 0);
    set_entry(EXCEPTION_NON_MASKABLE_INTERRUPT, handle_exception_non_maskable_interrupt, 0);
    set_entry(EXCEPTION_BREAKPOINT, handle_exception_breakpoint, 0);
    set_entry(EXCEPTION_OVERFLOW, handle_exception_overflow, 0);
    set_entry(EXCEPTION_BOUND_RANGE_EXCEEDED, handle_exception_bound_range_exceeded, 0);
    set_entry(EXCEPTION_INVALID_OPCODE, handle_exception_invalid_opcode, 0);
    set_entry(EXCEPTION_DEVICE_NOT_AVAILABLE, handle_exception_device_not_available, 0);
    set_entry(EXCEPTION_DOUBLE_FAULT, handle_exception_double_fault, 0);
    set_entry(EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN, handle_exception_coprocessor_segment_overrun, 0);
    set_entry(EXCEPTION_INVALID_TSS, handle_exception_invalid_tss, 0);
    set_entry(EXCEPTION_SEGMENT_NOT_PRESENT, handle_exception_segment_not_present, 0);
    set_entry(EXCEPTION_STACK_SEGMENT_FAULT, handle_exception_stack_segment_fault, 0);
    set_entry(EXCEPTION_GENERAL_PROTECTION_FAULT, handle_exception_general_protection_fault, 0);
    set_entry(EXCEPTION_PAGE_FAULT, handle_exception_page_fault, 0);
    set_entry(EXCEPTION_0xF, handle_exception_0xf, 0);
    set_entry(EXCEPTION_X87_FLOATING_POINT_EXCEPTION, handle_exception_x87_floating_point_exception, 0);
    set_entry(EXCEPTION_ALIGNMENT_CHECK, handle_exception_alignment_check, 0);
    set_entry(EXCEPTION_MACHINE_CHECK, handle_exception_machine_check, 0);
    set_entry(EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION, handle_exception_simd_floating_point_exception, 0);
    set_entry(EXCEPTION_VIRTUALIZATION_EXCEPTION, handle_exception_virtualization_exception, 0);
    set_entry(EXCEPTION_0x15, handle_exception_0x15, 0);
    set_entry(EXCEPTION_0x16, handle_exception_0x16, 0);
    set_entry(EXCEPTION_0x17, handle_exception_0x17, 0);
    set_entry(EXCEPTION_0x18, handle_exception_0x18, 0);
    set_entry(EXCEPTION_0x19, handle_exception_0x19, 0);
    set_entry(EXCEPTION_0x1A, handle_exception_0x1a, 0);
    set_entry(EXCEPTION_0x1B, handle_exception_0x1b, 0);
    set_entry(EXCEPTION_0x1C, handle_exception_0x1c, 0);
    set_entry(EXCEPTION_0x1D, handle_exception_0x1d, 0);
    set_entry(EXCEPTION_SECURITY_EXCEPTION, handle_exception_security_exception, 0);
    set_entry(EXCEPTION_0x1F, handle_exception_0x1f, 0);

    set_entry(IRQ_TIMER, handle_irq_timer, 0);
    set_entry(IRQ_KEYBOARD, handle_irq_keyboard, 0);
    set_entry(IRQ_CASCADE, handle_irq_cascade, 0);
    set_entry(IRQ_COM2, handle_irq_com2, 0);
    set_entry(IRQ_COM1, handle_irq_com1, 0);
    set_entry(IRQ_LPT2, handle_irq_lpt2, 0);
    set_entry(IRQ_FLOPPY, handle_irq_floppy, 0);
    set_entry(IRQ_LPT1, handle_irq_lpt1, 0);
    set_entry(IRQ_CMOS_REAL_TIME_CLOCK, handle_irq_cmos_real_time_clock, 0);
    set_entry(IRQ_PERIPHERALS_1, handle_irq_peripherals_1, 0);
    set_entry(IRQ_PERIPHERALS_2, handle_irq_peripherals_2, 0);
    set_entry(IRQ_PERIPHERALS_3, handle_irq_peripherals_3, 0);
    set_entry(IRQ_MOUSE, handle_irq_mouse, 0);
    set_entry(IRQ_COPRPCESSOR, handle_irq_coprpcessor, 0);
    set_entry(IRQ_PRIMARY_ATA, handle_irq_primary_ata, 0);
    set_entry(IRQ_SECONDARY_ATA, handle_irq_secondary_ata, 0);

    set_entry(IRQ_SYSCALL, handle_syscall, 0);

    pic_remap(IRQ_SYSCALL, IRQ_SYSCALL + 8);

    // actuall load the idt
	asm volatile("lidtl (%0)" : : "r"(&idt));
    asm("sti");

}

