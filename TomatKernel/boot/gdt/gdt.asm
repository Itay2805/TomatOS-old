[global gdt_set_segments]
gdt_set_segments:

    ; set the data registers
    mov eax, 16
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; far jump to the code segment
    jmp 8:gdt_set_segments_flush

gdt_set_segments_flush:
    ret