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
	[extern __cxa_finalize]

    [global bootloader]
	[global tomatokernel_stack]

    bootloader:
        mov esp, tomatokernel_stack
        push eax
        push ebx
        call kernel_boot

		sub esp, 4
		mov [esp], dword 0x0
		call __cxa_finalize
		add esp, 4

    bootloader_exit:
        cli
        hlt
        jmp bootloader_exit

[section .bss]
    resb 2 * 1024 * 1024
    tomatokernel_stack: