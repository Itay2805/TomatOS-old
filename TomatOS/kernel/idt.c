#include "idt.h"

void kernel_set_idt_gate(int n, uint32_t handler) {
	idt[n].low_offset = LOW16(handler);
	idt[n].sel = KERNEL_IDT_KERNEL_SEL;
	idt[n].zero = 0;
	idt[n].flags = 0x8E;
	idt[n].high_offset = HIGH16(handler);
}

void kernel_set_idt() {
	idt_reg.base = (uint32_t)&idt;
	idt_reg.limit = KERNEL_IDT_ENTRIES * sizeof(idt_gate_t) - 1;
#ifndef VISUAL_STUDIO
	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
#endif
}