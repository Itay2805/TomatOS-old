[extern kernel_exception_handler]
[extern kernel_irq_handler]

CODE_SEGMENT equ 8
DATA_SEGMENT equ 16

global ignore_interrupt
ignore_interrupt:
	cli
	sti
	iret

global handle_syscall
handle_syscall:
    cli
    push byte 0
    push 0x80
    jmp irq_common_stub

global handle_exception_interrupt_00
global handle_exception_interrupt_01
global handle_exception_interrupt_02
global handle_exception_interrupt_03
global handle_exception_interrupt_04
global handle_exception_interrupt_05
global handle_exception_interrupt_06
global handle_exception_interrupt_07
global handle_exception_interrupt_08
global handle_exception_interrupt_09
global handle_exception_interrupt_0A
global handle_exception_interrupt_0B
global handle_exception_interrupt_0C
global handle_exception_interrupt_0D
global handle_exception_interrupt_0E
global handle_exception_interrupt_0F
global handle_exception_interrupt_10
global handle_exception_interrupt_11
global handle_exception_interrupt_12
global handle_exception_interrupt_13
global handle_exception_interrupt_14
global handle_exception_interrupt_15
global handle_exception_interrupt_16
global handle_exception_interrupt_17
global handle_exception_interrupt_18
global handle_exception_interrupt_19
global handle_exception_interrupt_1A
global handle_exception_interrupt_1B
global handle_exception_interrupt_1C
global handle_exception_interrupt_1D
global handle_exception_interrupt_1E
global handle_exception_interrupt_1F

global handle_interrupt_request_00
global handle_interrupt_request_01
global handle_interrupt_request_02
global handle_interrupt_request_03
global handle_interrupt_request_04
global handle_interrupt_request_05
global handle_interrupt_request_06
global handle_interrupt_request_07
global handle_interrupt_request_08
global handle_interrupt_request_09
global handle_interrupt_request_0A
global handle_interrupt_request_0B
global handle_interrupt_request_0C
global handle_interrupt_request_0D
global handle_interrupt_request_0E
global handle_interrupt_request_0F

exception_common_stub:
	pusha
	mov ax, ds
	push eax
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call kernel_exception_handler

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8
	sti
	iret

irq_common_stub:
	pusha
    mov ax, ds
    push eax
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kernel_irq_handler 
    
	pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret 

handle_exception_interrupt_00:
    cli
    push byte 0
    push byte 0
    jmp exception_common_stub

handle_exception_interrupt_01:
    cli
    push byte 0
    push byte 1
    jmp exception_common_stub

handle_exception_interrupt_02:
    cli
    push byte 0
    push byte 2
    jmp exception_common_stub

handle_exception_interrupt_03:
    cli
    push byte 0
    push byte 3
    jmp exception_common_stub

handle_exception_interrupt_04:
    cli
    push byte 0
    push byte 4
    jmp exception_common_stub

handle_exception_interrupt_05:
    cli
    push byte 0
    push byte 5
    jmp exception_common_stub

handle_exception_interrupt_06:
    cli
    push byte 0
    push byte 6
    jmp exception_common_stub

handle_exception_interrupt_07:
    cli
    push byte 0
    push byte 7
    jmp exception_common_stub

handle_exception_interrupt_08:
    cli
    push byte 8
    jmp exception_common_stub

handle_exception_interrupt_09:
    cli
    push byte 0
    push byte 9
    jmp exception_common_stub

handle_exception_interrupt_0A:
    cli
    push byte 10
    jmp exception_common_stub

handle_exception_interrupt_0B:
    cli
    push byte 11
    jmp exception_common_stub

handle_exception_interrupt_0C:
    cli
    push byte 12
    jmp exception_common_stub

handle_exception_interrupt_0D:
    cli
    push byte 13
    jmp exception_common_stub

handle_exception_interrupt_0E:
    cli
    push byte 14
    jmp exception_common_stub

handle_exception_interrupt_0F:
    cli
    push byte 0
    push byte 15
    jmp exception_common_stub

handle_exception_interrupt_10:
    cli
    push byte 0
    push byte 16
    jmp exception_common_stub

handle_exception_interrupt_11:
    cli
    push byte 0
    push byte 17
    jmp exception_common_stub

handle_exception_interrupt_12:
    cli
    push byte 0
    push byte 18
    jmp exception_common_stub

handle_exception_interrupt_13:
    cli
    push byte 0
    push byte 19
    jmp exception_common_stub

handle_exception_interrupt_14:
    cli
    push byte 0
    push byte 20
    jmp exception_common_stub

handle_exception_interrupt_15:
    cli
    push byte 0
    push byte 21
    jmp exception_common_stub

handle_exception_interrupt_16:
    cli
    push byte 0
    push byte 22
    jmp exception_common_stub

handle_exception_interrupt_17:
    cli
    push byte 0
    push byte 23
    jmp exception_common_stub

handle_exception_interrupt_18:
    cli
    push byte 0
    push byte 24
    jmp exception_common_stub

handle_exception_interrupt_19:
    cli
    push byte 0
    push byte 25
    jmp exception_common_stub

handle_exception_interrupt_1A:
    cli
    push byte 0
    push byte 26
    jmp exception_common_stub

handle_exception_interrupt_1B:
    cli
    push byte 0
    push byte 27
    jmp exception_common_stub

handle_exception_interrupt_1C:
    cli
    push byte 0
    push byte 28
    jmp exception_common_stub

handle_exception_interrupt_1D:
    cli
    push byte 0
    push byte 29
    jmp exception_common_stub

handle_exception_interrupt_1E:
    cli
    push byte 30
    jmp exception_common_stub

handle_exception_interrupt_1F:
    cli
    push byte 0
    push byte 31
    jmp exception_common_stub

;;;;;;; IRQs

handle_interrupt_request_00:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

handle_interrupt_request_01:
	cli
	push byte 1
	push byte 33
	jmp irq_common_stub

handle_interrupt_request_02:
	cli
	push byte 2
	push byte 34
	jmp irq_common_stub

handle_interrupt_request_03:
	cli
	push byte 3
	push byte 35
	jmp irq_common_stub

handle_interrupt_request_04:
	cli
	push byte 4
	push byte 36
	jmp irq_common_stub

handle_interrupt_request_05:
	cli
	push byte 5
	push byte 37
	jmp irq_common_stub

handle_interrupt_request_06:
	cli
	push byte 6
	push byte 38
	jmp irq_common_stub

handle_interrupt_request_07:
	cli
	push byte 7
	push byte 39
	jmp irq_common_stub

handle_interrupt_request_08:
	cli
	push byte 8
	push byte 40
	jmp irq_common_stub

handle_interrupt_request_09:
	cli
	push byte 9
	push byte 41
	jmp irq_common_stub

handle_interrupt_request_0A:
	cli
	push byte 10
	push byte 42
	jmp irq_common_stub

handle_interrupt_request_0B:
	cli
	push byte 11
	push byte 43
	jmp irq_common_stub

handle_interrupt_request_0C:
	cli
	push byte 12
	push byte 44
	jmp irq_common_stub

handle_interrupt_request_0D:
	cli
	push byte 13
	push byte 45
	jmp irq_common_stub

handle_interrupt_request_0E:
	cli
	push byte 14
	push byte 46
	jmp irq_common_stub

handle_interrupt_request_0F:
	cli
	push byte 15
	push byte 47
	jmp irq_common_stub