#include "kernel.h"

#include <stdbool.h>

#include <core/graphics/term.h>
#include <core/memory/heap.h>
#include <core/process/process.h>

void kpanic(char* error) {
	term_set_text_color(0xf);
	term_set_background_color(0x8);

	term_write("kernel panic: \n");
	term_write(error);

	while (true) {
		asm("hlt");
		asm("nop");
	}
}

heap_context_t* kheap(void) {
	return &process_get(0)->heap;
}

void outb(uint16_t port, uint8_t data) {
	__asm__ volatile("outb %0, %1" : : "a"(data), "Nd" (port));
}

uint8_t inb(uint16_t port) {
	uint8_t result = 0;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void outw(uint16_t port, uint16_t data) {
	__asm__ volatile("outw %0, %1" : : "a"(data), "Nd" (port));
}

uint16_t inw(uint16_t port) {
	uint16_t result = 0;
	__asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

void outl(uint16_t port, uint32_t data) {
	__asm__ volatile("outl %0, %1" : : "a"(data), "Nd" (port));
}

uint32_t inl(uint16_t port) {
	uint32_t result = 0;
	__asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}