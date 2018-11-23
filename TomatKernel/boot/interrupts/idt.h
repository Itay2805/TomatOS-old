#ifndef BOOT_IDT_H
#define BOOT_IDT_H

#include <stdint.h>

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IDT_TASK_GATE           0x5
#define IDT_INTERRUPT_GATE_16   0x6
#define IDT_TRAP_GATE_16        0x7
#define IDT_INTERRUPT_GATE      0xE
#define IDT_TRAP_GATE           0xF

typedef struct idt_gate {
    uint16_t handler_address_low;
    uint16_t selector;
    uint8_t unused;
    uint8_t gate_type : 4;
    uint8_t storage_segment : 1;
    uint8_t DPL : 2;
    uint8_t present : 1;
    uint16_t handler_address_high;
} PACKED idt_gate_t;

typedef struct idt {
    uint16_t size;
    idt_gate_t* gates; 
} PACKED idt_t;

extern idt_t idt;

typedef void(*idt_handle_t)();

void idt_init(void);
void idt_set_entry(int index, idt_handle_t base, uint8_t ring);

#ifdef __cplusplus
}
#endif

#endif