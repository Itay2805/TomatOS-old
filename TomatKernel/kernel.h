#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

// The PACKED macro will make so the struct won't be padded
// useful for structures that must have no padding
#ifdef __GNUC__
	#define PACKED __attribute__((packed))
#else
	#error Must use GCC (GNUC) for for the PACKED macro to work
#endif

#define KB(x) ((x) * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

// this is going to be used alot mainly in the interrupts and processes
typedef struct registers {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, code;
	uint32_t eip, cs, eflags, useresp, ss;
} PACKED registers_t;

// the *addresses* of these symbols are the start, end and size of the kernel 
// respectively 
extern void* kernel_start;
extern void* kernel_end;
extern void* kernel_size;

// all the different port in/outs

#define outb(port, data) asm("outb %0, %1" : : "a"((uint8_t)data), "Nd"((uint16_t)port))
#define outw(port, data) asm("outw %0, %1" : : "a"((uint16_t)data), "Nd"((uint16_t)port))
#define outl(port, data) asm("outl %0, %1" : : "a"((uint32_t)data), "Nd"((uint16_t)port))

uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

// read/write Model Specific Register

#define wrmsr(msr, value) asm volatile( "wrmsr" : : "c"((uint32_t)msr), "A"((uint64_t)value)
uint64_t rdmsr(uint32_t msr);

// time-stamp counter
uint64_t rdtsc();

#endif