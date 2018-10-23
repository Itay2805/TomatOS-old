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

// all the different port optimizations

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);

void outw(uint16_t port, uint16_t data);
uint16_t inw(uint16_t port);

void outl(uint16_t port, uint32_t data);
uint32_t inl(uint16_t port);

#endif