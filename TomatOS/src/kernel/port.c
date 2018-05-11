#include "port.h"
#include "common.h"

void port_write8(uint16_t port, uint8_t data) {
	ASM(__asm__ volatile("outb %0, %1" : : "a"(data), "Nd" (port)));
}

uint8_t port_read8(uint16_t port) {
	uint8_t result;
	ASM(__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port)));
	return result;
}

void port_write16(uint16_t port, uint16_t data) {
	ASM(__asm__ volatile("outw %0, %1" : : "a"(data), "Nd" (port)));
}

uint16_t port_read16(uint16_t port) {
	uint16_t result;
	ASM(__asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port)));
	return result;
}

void port_write32(uint16_t port, uint32_t data) {
	ASM(__asm__ volatile("outl %0, %1" : : "a"(data), "Nd" (port)));
}

uint32_t port_read32(uint16_t port) {
	uint32_t result;
	ASM(__asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port)));
	return result;
}