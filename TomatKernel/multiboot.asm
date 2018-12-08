[BITS 32]

MAGIC equ 0x1badb002

; the different available flags
; 4k align the kernel image
; provide memory information
FLAGS equ ((1 << 0) | (1 << 1))
CHECKSUM equ -(MAGIC + FLAGS)

[section .multiboot]
    ; header
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

[section .text]
    [extern kmain]

    [global bootloader]
	[global kernel_stack]

    bootloader:
        mov esp, kernel_stack
        push eax
        push ebx
        call kmain
        add esp, 4

[section .bss]
    resb 2 * 1024 * 1024
    kernel_stack: