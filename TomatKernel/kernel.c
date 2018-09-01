#include "kernel.h"

#include <stdbool.h>

#include <core/term.h>
#include <core/memory/heap.h>

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

static heap_context_t kernel_heap;

heap_context_t* kheap(void) {
	return &kernel_heap;
}



void outb(uint16_t port, uint8_t data) {
#ifndef _MBCS
	asm volatile("outb %0, %1" : : "a"(data), "Nd" (port));
#endif
}

uint8_t inb(uint16_t port) {
	uint8_t result = 0;
#ifndef _MBCS
	asm volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
#endif
	return result;
}

void outw(uint16_t port, uint16_t data) {
#ifndef _MBCS
	asm volatile("outw %0, %1" : : "a"(data), "Nd" (port));
#endif
}

uint16_t inw(uint16_t port) {
	uint16_t result = 0;
#ifndef _MBCS
	asm volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
#endif
	return result;
}

void outl(uint16_t port, uint32_t data) {
#ifndef _MBCS
	asm volatile("outl %0, %1" : : "a"(data), "Nd" (port));
#endif
}

uint32_t inl(uint16_t port) {
	uint32_t result = 0;
#ifndef _MBCS
	asm volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
#endif
	return result;
}