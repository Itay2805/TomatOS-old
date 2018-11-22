[extern gdt]

; will setup a the Kernel GDT
; This should eventually be part of TomatBoot
[global gdt_init]
gdt_init:
    ; load the gdt
    lgdt [gdt]

    ; set the segment registers
    mov eax, 16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; this will set the code register
    jmp 8:gdt_init_ret

gdt_init_ret:
    ret
