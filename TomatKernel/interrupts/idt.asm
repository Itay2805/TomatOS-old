[extern kernel_exception_handler]
[extern kernel_irq_handler]

CODE_SEGMENT equ 8
DATA_SEGMENT equ 16

global ignore_interrupt
ignore_interrupt:
    push ebp
	cli
	sti
    pop ebp
	iret

global handle_syscall
handle_syscall:
    push ebp
    cli
    push byte 0
    push 0x80
    jmp irq_common_stub

global handle_exception_divide_by_zero
global handle_exception_debug
global handle_exception_non_maskable_interrupt
global handle_exception_breakpoint
global handle_exception_overflow
global handle_exception_bound_range_exceeded
global handle_exception_invalid_opcode
global handle_exception_device_not_available
global handle_exception_double_fault
global handle_exception_coprocessor_segment_overrun
global handle_exception_invalid_tss
global handle_exception_segment_not_present
global handle_exception_stack_segment_fault
global handle_exception_general_protection_fault
global handle_exception_page_fault
global handle_exception_0xf
global handle_exception_x87_floating_point_exception
global handle_exception_alignment_check
global handle_exception_machine_check
global handle_exception_simd_floating_point_exception
global handle_exception_virtualization_exception
global handle_exception_0x15
global handle_exception_0x16
global handle_exception_0x17
global handle_exception_0x18
global handle_exception_0x19
global handle_exception_0x1a
global handle_exception_0x1b
global handle_exception_0x1c
global handle_exception_0x1d
global handle_exception_security_exception
global handle_exception_0x1f

global handle_irq_timer
global handle_irq_keyboard
global handle_irq_cascade
global handle_irq_com2
global handle_irq_com1
global handle_irq_lpt2
global handle_irq_floppy
global handle_irq_lpt1
global handle_irq_cmos_real_time_clock
global handle_irq_peripherals_1
global handle_irq_peripherals_2
global handle_irq_peripherals_3
global handle_irq_mouse
global handle_irq_coprpcessor
global handle_irq_primary_ata
global handle_irq_secondary_ata

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

    pop ebp
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

    pop ebp
    iret 

handle_exception_divide_by_zero:
    push ebp
    cli
    push byte 0
    push byte 0
    jmp exception_common_stub

handle_exception_debug:
    push ebp
    cli
    push byte 0
    push byte 1
    jmp exception_common_stub

handle_exception_non_maskable_interrupt:
    push ebp
    cli
    push byte 0
    push byte 2
    jmp exception_common_stub

handle_exception_breakpoint:
    push ebp
    cli
    push byte 0
    push byte 3
    jmp exception_common_stub

handle_exception_overflow:
    push ebp
    cli
    push byte 0
    push byte 4
    jmp exception_common_stub

handle_exception_bound_range_exceeded:
    push ebp
    cli
    push byte 0
    push byte 5
    jmp exception_common_stub

handle_exception_invalid_opcode:
    push ebp
    cli
    push byte 0
    push byte 6
    jmp exception_common_stub

handle_exception_device_not_available:
    push ebp
    cli
    push byte 0
    push byte 7
    jmp exception_common_stub

handle_exception_double_fault:
    push ebp
    cli
    push byte 8
    jmp exception_common_stub

handle_exception_coprocessor_segment_overrun:
    push ebp
    cli
    push byte 0
    push byte 9
    jmp exception_common_stub

handle_exception_invalid_tss:
    push ebp
    cli
    push byte 10
    jmp exception_common_stub

handle_exception_segment_not_present:
    push ebp
    cli
    push byte 11
    jmp exception_common_stub

handle_exception_stack_segment_fault:
    push ebp
    cli
    push byte 12
    jmp exception_common_stub

handle_exception_general_protection_fault:
    push ebp
    cli
    push byte 13
    jmp exception_common_stub

handle_exception_page_fault:
    push ebp
    cli
    push byte 14
    jmp exception_common_stub

handle_exception_0xf:
    push ebp
    cli
    push byte 0
    push byte 15
    jmp exception_common_stub

handle_exception_x87_floating_point_exception:
    push ebp
    cli
    push byte 0
    push byte 16
    jmp exception_common_stub

handle_exception_alignment_check:
    push ebp
    cli
    push byte 0
    push byte 17
    jmp exception_common_stub

handle_exception_machine_check:
    push ebp
    cli
    push byte 0
    push byte 18
    jmp exception_common_stub

handle_exception_simd_floating_point_exception:
    push ebp
    cli
    push byte 0
    push byte 19
    jmp exception_common_stub

handle_exception_virtualization_exception:
    push ebp
    cli
    push byte 0
    push byte 20
    jmp exception_common_stub

handle_exception_0x15:
    push ebp
    cli
    push byte 0
    push byte 21
    jmp exception_common_stub

handle_exception_0x16:
    push ebp
    cli
    push byte 0
    push byte 22
    jmp exception_common_stub

handle_exception_0x17:
    push ebp
    cli
    push byte 0
    push byte 23
    jmp exception_common_stub

handle_exception_0x18:
    push ebp
    cli
    push byte 0
    push byte 24
    jmp exception_common_stub

handle_exception_0x19:
    push ebp
    cli
    push byte 0
    push byte 25
    jmp exception_common_stub

handle_exception_0x1a:
    push ebp
    cli
    push byte 0
    push byte 26
    jmp exception_common_stub

handle_exception_0x1b:
    push ebp
    cli
    push byte 0
    push byte 27
    jmp exception_common_stub

handle_exception_0x1c:
    push ebp
    cli
    push byte 0
    push byte 28
    jmp exception_common_stub

handle_exception_0x1d:
    push ebp
    cli
    push byte 0
    push byte 29
    jmp exception_common_stub

handle_exception_security_exception:
    push ebp
    cli
    push byte 30
    jmp exception_common_stub

handle_exception_0x1f:
    push ebp
    cli
    push byte 0
    push byte 31
    jmp exception_common_stub

;;;;;;; IRQs

handle_irq_timer:
    push ebp
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

handle_irq_keyboard:
    push ebp
	cli
	push byte 1
	push byte 33
	jmp irq_common_stub

handle_irq_cascade:
    push ebp
	cli
	push byte 2
	push byte 34
	jmp irq_common_stub

handle_irq_com2:
    push ebp
	cli
	push byte 3
	push byte 35
	jmp irq_common_stub

handle_irq_com1:
    push ebp
	cli
	push byte 4
	push byte 36
	jmp irq_common_stub

handle_irq_lpt2:
    push ebp
	cli
	push byte 5
	push byte 37
	jmp irq_common_stub

handle_irq_floppy:
    push ebp
	cli
	push byte 6
	push byte 38
	jmp irq_common_stub

handle_irq_lpt1:
    push ebp
	cli
	push byte 7
	push byte 39
	jmp irq_common_stub

handle_irq_cmos_real_time_clock:
    push ebp
	cli
	push byte 8
	push byte 40
	jmp irq_common_stub

handle_irq_peripherals_1:
    push ebp
	cli
	push byte 9
	push byte 41
	jmp irq_common_stub

handle_irq_peripherals_2:
    push ebp
	cli
	push byte 10
	push byte 42
	jmp irq_common_stub

handle_irq_peripherals_3:
    push ebp
	cli
	push byte 11
	push byte 43
	jmp irq_common_stub

handle_irq_mouse:
    push ebp
	cli
	push byte 12
	push byte 44
	jmp irq_common_stub

handle_irq_coprpcessor:
    push ebp
	cli
	push byte 13
	push byte 45
	jmp irq_common_stub

handle_irq_primary_ata:
    push ebp
	cli
	push byte 14
	push byte 46
	jmp irq_common_stub

handle_irq_secondary_ata:
    push ebp
	cli
	push byte 15
	push byte 47
	jmp irq_common_stub
