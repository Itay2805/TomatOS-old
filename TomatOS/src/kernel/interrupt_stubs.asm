[BITS 32]

IRQ_OFFSET equ 32

[section .text]
[extern handle_interrupt]

; default interrupt
[global ignore_interrupt]
ignore_interrupt:
	iret

; exception handlers
[global handle_exception_interrupt_00]
[global handle_exception_interrupt_01]
[global handle_exception_interrupt_02]
[global handle_exception_interrupt_03]
[global handle_exception_interrupt_04]
[global handle_exception_interrupt_05]
[global handle_exception_interrupt_06]
[global handle_exception_interrupt_07]
[global handle_exception_interrupt_08]
[global handle_exception_interrupt_09]
[global handle_exception_interrupt_0A]
[global handle_exception_interrupt_0B]
[global handle_exception_interrupt_0C]
[global handle_exception_interrupt_0D]
[global handle_exception_interrupt_0E]
[global handle_exception_interrupt_0F]
[global handle_exception_interrupt_10]
[global handle_exception_interrupt_11]
[global handle_exception_interrupt_12]

handle_exception_interrupt_00:
	cli
	push byte 0
	push byte 0
	jmp exception_common_handle
handle_exception_interrupt_01:
	cli
	push byte 0
	push byte 1
	jmp exception_common_handle
handle_exception_interrupt_02:
	cli
	push byte 0
	push byte 2
	jmp exception_common_handle
handle_exception_interrupt_03:
	cli
	push byte 0
	push byte 3
	jmp exception_common_handle
handle_exception_interrupt_04:
	cli
	push byte 0
	push byte 4
	jmp exception_common_handle
handle_exception_interrupt_05:
	cli
	push byte 0
	push byte 5
	jmp exception_common_handle
handle_exception_interrupt_06:
	cli
	push byte 0
	push byte 6
	jmp exception_common_handle
handle_exception_interrupt_07:
	cli
	push byte 0
	push byte 7
	jmp exception_common_handle
handle_exception_interrupt_08:
	cli
	push byte 8
	jmp exception_common_handle
handle_exception_interrupt_09:
	cli
	push byte 0
	push byte 9
	jmp exception_common_handle
handle_exception_interrupt_0A:
	cli
	push byte 10
	jmp exception_common_handle
handle_exception_interrupt_0B:
	cli
	push byte 11
	jmp exception_common_handle
handle_exception_interrupt_0C:
	cli
	push byte 12
	jmp exception_common_handle
handle_exception_interrupt_0D:
	cli
	push byte 13
	jmp exception_common_handle
handle_exception_interrupt_0E:
	cli
	push byte 14
	jmp exception_common_handle
handle_exception_interrupt_0F:
	cli
	push byte 0
	push byte 15
	jmp exception_common_handle
handle_exception_interrupt_10:
	cli
	push byte 0
	push byte 16
	jmp exception_common_handle
handle_exception_interrupt_11:
	cli
	push byte 0
	push byte 17
	jmp exception_common_handle
handle_exception_interrupt_12:
	cli
	push byte 0
	push byte 18
	jmp exception_common_handle

; irq handlers
[global handle_interrupt_request_00]
[global handle_interrupt_request_01]
[global handle_interrupt_request_02]
[global handle_interrupt_request_03]
[global handle_interrupt_request_04]
[global handle_interrupt_request_05]
[global handle_interrupt_request_06]
[global handle_interrupt_request_07]
[global handle_interrupt_request_08]
[global handle_interrupt_request_09]
[global handle_interrupt_request_0A]
[global handle_interrupt_request_0B]
[global handle_interrupt_request_0C]
[global handle_interrupt_request_0D]
[global handle_interrupt_request_0E]
[global handle_interrupt_request_0F]

handle_interrupt_request_00:
	cli
	push byte 0
	push byte 32
	jmp irq_common_handle
handle_interrupt_request_01:
	cli
	push byte 1
	push byte 33
	jmp irq_common_handle
handle_interrupt_request_02:
	cli
	push byte 2
	push byte 34
	jmp irq_common_handle
handle_interrupt_request_03:
	cli
	push byte 3
	push byte 35
	jmp irq_common_handle
handle_interrupt_request_04:
	cli
	push byte 4
	push byte 36
	jmp irq_common_handle
handle_interrupt_request_05:
	cli
	push byte 5
	push byte 37
	jmp irq_common_handle
handle_interrupt_request_06:
	cli
	push byte 6
	push byte 38
	jmp irq_common_handle
handle_interrupt_request_07:
	push byte 7
	cli
	push byte 39
	jmp irq_common_handle
handle_interrupt_request_08:
	push byte 8
	cli
	push byte 40
	jmp irq_common_handle
handle_interrupt_request_09:
	push byte 9
	cli
	push byte 41
	jmp irq_common_handle
handle_interrupt_request_0A:
	push byte 10
	cli
	push byte 42
	jmp irq_common_handle
handle_interrupt_request_0B:
	cli
	push byte 11
	push byte 43
	jmp irq_common_handle
handle_interrupt_request_0C:
	cli
	push byte 12
	push byte 44
	jmp irq_common_handle
handle_interrupt_request_0D:
	cli
	push byte 13
	push byte 45
	jmp irq_common_handle
handle_interrupt_request_0E:
	cli
	push byte 14
	push byte 46
	jmp irq_common_handle
handle_interrupt_request_0F:
	cli
	push byte 15
	push byte 47
	jmp irq_common_handle

; syscall
[global handle_syscall]

handle_syscall:
	cli
	push byte 0
	push byte 128
	jmp irq_common_handle

; interrupt handling code
irq_common_handle:
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call handle_interrupt
	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	popa
	add esp, 8
	sti
	iret

exception_common_handle:
	pusha
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call handle_interrupt
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8
	sti
	iret