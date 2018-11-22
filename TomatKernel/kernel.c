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

// this is the entry point of the kernel
void kmain() {

}
