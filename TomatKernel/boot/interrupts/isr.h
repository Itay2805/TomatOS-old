#ifndef BOOT_ISR_H
#define BOOT_ISR_H

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

// default interrupt
extern void ignore_interrupt();

// syscall interrupt
extern void handle_syscall();

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
#define EXCEPTION_DIVIDE_BY_ZERO					0x00
#define EXCEPTION_DEBUG								0x01
#define EXCEPTION_NON_MASKABLE_INTERRUPT			0x02
#define EXCEPTION_BREAKPOINT						0x03
#define EXCEPTION_OVERFLOW							0x04
#define EXCEPTION_BOUND_RANGE_EXCEEDED				0x05
#define EXCEPTION_INVALID_OPCODE					0x06
#define EXCEPTION_DEVICE_NOT_AVAILABLE				0x07
#define EXCEPTION_DOUBLE_FAULT						0x08
#define EXCEPTION_COPROCESSOR_SEGMENT_OVERRUN		0x09
#define EXCEPTION_INVALID_TSS						0x0A
#define EXCEPTION_SEGMENT_NOT_PRESENT				0x0B
#define EXCEPTION_STACK_SEGMENT_FAULT				0x0C
#define EXCEPTION_GENERAL_PROTECTION_FAULT			0x0D
#define EXCEPTION_PAGE_FAULT						0x0E
#define EXCEPTION_0xF								0x0F
#define EXCEPTION_X87_FLOATING_POINT_EXCEPTION		0x10
#define EXCEPTION_ALIGNMENT_CHECK					0x11
#define EXCEPTION_MACHINE_CHECK						0x12
#define EXCEPTION_SIMD_FLOATING_POINT_EXCEPTION		0x13
#define EXCEPTION_VIRTUALIZATION_EXCEPTION			0x14
#define EXCEPTION_0x15								0x15
#define EXCEPTION_0x16								0x16
#define EXCEPTION_0x17								0x17
#define EXCEPTION_0x18								0x18
#define EXCEPTION_0x19								0x19
#define EXCEPTION_0x1A								0x1A
#define EXCEPTION_0x1B								0x1B
#define EXCEPTION_0x1C								0x1C
#define EXCEPTION_0x1D								0x1D
#define EXCEPTION_SECURITY_EXCEPTION				0x1E
#define EXCEPTION_0x1F								0x1F

extern void handle_exception_interrupt_00();
extern void handle_exception_interrupt_01();
extern void handle_exception_interrupt_02();
extern void handle_exception_interrupt_03();
extern void handle_exception_interrupt_04();
extern void handle_exception_interrupt_05();
extern void handle_exception_interrupt_06();
extern void handle_exception_interrupt_07();
extern void handle_exception_interrupt_08();	// ERROR CODE
extern void handle_exception_interrupt_09();
extern void handle_exception_interrupt_0A();	// ERROR CODE
extern void handle_exception_interrupt_0B();	// ERROR CODE
extern void handle_exception_interrupt_0C();	// ERROR CODE
extern void handle_exception_interrupt_0D();	// ERROR CODE
extern void handle_exception_interrupt_0E();	// ERROR CODE
extern void handle_exception_interrupt_0F();
extern void handle_exception_interrupt_10();
extern void handle_exception_interrupt_11();	// ERROR CODE
extern void handle_exception_interrupt_12();
extern void handle_exception_interrupt_13();
extern void handle_exception_interrupt_14();
extern void handle_exception_interrupt_15();
extern void handle_exception_interrupt_16();
extern void handle_exception_interrupt_17();
extern void handle_exception_interrupt_18();
extern void handle_exception_interrupt_19();
extern void handle_exception_interrupt_1A();
extern void handle_exception_interrupt_1B();
extern void handle_exception_interrupt_1C();
extern void handle_exception_interrupt_1D();
extern void handle_exception_interrupt_1E();	// ERROR CODE
extern void handle_exception_interrupt_1F();

void isr_init(void);

typedef void(*irq_handler_t)(registers_t* regs);
void register_irq_handler(uint8_t num, irq_handler_t handler);

void kernel_exception_handler(registers_t regs);
void kernel_irq_handler(registers_t regs);

#ifdef __cplusplus
}
#endif

#endif