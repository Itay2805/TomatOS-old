[BITS 32]

MAGIC equ 0x1badb002
FLAGS equ (1 << 0 | 1 << 1)
CHECKSUM equ -(MAGIC + FLAGS)

; multiboot, continas the data needed for the multiboot
[section .multiboot]
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

; code section, start of bootloader
[section .text]
	[extern kmain]
	[extern callConstructors]
	; [extern __cxa_finalize]
	[global bootloader]

	; bootloader, will set the kernel stack and call the kmain function
	bootloader:
		mov esp, kernel_stack
		call callConstructors
		push eax
		push ebx
		call kmain

	bootloader_exit:
		; C++ atexit
		;sub esp, 4
		;mov [esp], dword 0x0
		;call __cxa_finalize
		;add esp, 4

		cli 
		hlt
		jmp bootloader_exit

; allocate for the kernel 2MB of stack
[section .bss]
	resb 2*1024*1024
	kernel_stack:
