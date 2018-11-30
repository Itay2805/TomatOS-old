#include "kernel.h"

void outb(uint16_t port, uint8_t data) {
	asm("outb %0, %1" : : "a"(data), "Nd" (port));
}

uint8_t inb(uint16_t port) {
	uint8_t result = 0;
	asm("inb %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void outw(uint16_t port, uint16_t data) {
	asm("outw %0, %1" : : "a"(data), "Nd" (port));
}

uint16_t inw(uint16_t port) {
	uint16_t result = 0;
	asm("inw %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void outl(uint16_t port, uint32_t data) {
	asm("outl %0, %1" : : "a"(data), "Nd" (port));
}

uint32_t inl(uint16_t port) {
	uint32_t result = 0;
	asm("inl %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void wrmsr(uint32_t msr, uint64_t value) {
	asm volatile( "wrmsr" : : "c" (msr), "A" (value) );
}

uint64_t rdmsr(uint32_t msr) {
	uint64_t result = 0;
	asm volatile ( "rdmsr" : "=A" (result) : "c" (msr) );
	return result;
}

uint64_t rdtsc() {
	uint64_t result = 0;
    asm volatile ( "rdtsc" : "=A"(result) );
	return result;
}

#include <stdbool.h>

#include <gdt/gdt.h>
#include <interrupts/idt.h>
#include <multiboot.h>

// this is the entry point of the kernel
void kmain(multiboot_t* multiboot_structure, uint32_t multiboot_magic) {
	// Make sure we booted properly
	if(multiboot_magic != 0x2BADB002) {
		// Print error
		while(true) asm("hlt");
	}
	idt_init();

	// TODO: use multiboot memory information to setup stuff
}
