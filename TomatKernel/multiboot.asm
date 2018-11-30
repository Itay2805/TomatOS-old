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

    ; graphics information:
    ;   mode_type
    ;   width
    ;   height
    ;   bits-per-pixel
    dd 0 ; linear graphics mode
    dd 1280
    dd 720
    dd 32

[section .text]
    [extern kmain]

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