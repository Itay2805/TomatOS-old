#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include "kernel.h"

#include <stdint.h>

#define KERNEL_IDT_KERNEL_SEL 0x08

#define KERNEL_IDT_ENTRIES 256

typedef struct {
	uint16_t low_offset;
	uint16_t sel;
	uint8_t zero;
	uint8_t flags;
	uint16_t high_offset;
}
#ifndef VISUAL_STUDIO
// visual studio doesn't like this
__attribute__((packed))
#endif
idt_gate_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
}
#ifndef VISUAL_STUDIO
// visual studio doesn't like this
__attribute__((packed))
#endif
idt_register_t;

idt_gate_t idt[KERNEL_IDT_ENTRIES];
idt_register_t idt_reg;

void kernel_set_idt_gate(int n, uint32_t handler);
void kernel_set_idt();

#endif