gdt_start:
    dd 0x0
    dd 0x0

; GDT for code segment
gdt_code:
    dw 0xffff       ; segment length
    dw 0x0          ; segment base
    db 0x0          ; segment base
    db 10011010b    ; flags
    db 11001111b    ; flags + segment length
    db 0x0          ; segment base

; GDT for data segment
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size
    dd gdt_start                ; address

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
