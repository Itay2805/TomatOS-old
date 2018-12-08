#include "kernel.h"

uint8_t inb(uint16_t port) {
	uint8_t result = 0;
	asm("inb %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

uint16_t inw(uint16_t port) {
	uint16_t result = 0;
	asm("inw %1, %0" : "=a" (result) : "Nd" (port));
	return result;
}

uint32_t inl(uint16_t port) {
	uint32_t result = 0;
	asm("inl %1, %0" : "=a" (result) : "Nd" (port));
	return result;
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
#include <memory/paging.h>
#include <multiboot.h>

// this is the entry point of the kernel
void kmain(multiboot_t* multiboot_structure, uint32_t multiboot_magic) {
	// Make sure we booted properly
	if(multiboot_magic != 0x2BADB002) {
		// Print error
		while(true) asm("hlt");
	}
	idt_init();

	asm("int $3");

	paging_init();

	// test paging with some text stuff
	page_directory_t pd = paging_allocate_new_page();
	paging_init_directory(pd);
	paging_set(pd);
	paging_enable();
	uint8_t* vga_text = (void*)0xb8000;
	vga_text[0] = 'A';

	while(true) {
		asm("hlt");
	}
	// TODO: use multiboot memory information to setup stuff
}
