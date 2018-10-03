[BITS 32]

MAGIC equ 0x1badb002
FLAGS equ (1 << 0 | 1 << 1)
CHECKSUM equ -(MAGIC + FLAGS)

[section .multiboot]
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

[section .text]
    [extern kernel_boot]

    [global bootloader]
	[global tomatokernel_stack]

    bootloader:
        mov esp, tomatokernel_stack
        push eax
        push ebx
        call kernel_boot

[section .bss]
    resb 2 * 1024 * 1024
    tomatokernel_stack: