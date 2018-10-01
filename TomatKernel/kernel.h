#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

// just so VisualStudio will continue to have nice syntax highlighting 
#define PACKED __attribute__((packed))

typedef struct registers {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, code;
	uint32_t eip, cs, eflags, useresp, ss;
} PACKED registers_t;

#ifdef __cplusplus
extern "C" {
#endif

	extern void* tomatkernel_start;
	extern void* tomatkernel_end;
	extern void* tomatkernel_size;

	// stack start
	extern char* tomatokernel_stack;

    void outb(uint16_t port, uint8_t data);
    uint8_t inb(uint16_t port);

    void outw(uint16_t port, uint16_t data);
    uint16_t inw(uint16_t port);

    void outl(uint16_t port, uint32_t data);
    uint32_t inl(uint16_t port);

	struct heap_context* kheap(void);

	void kpanic(char* error);
	extern void kfault(void);

#ifdef __cplusplus
}
#endif

#endif