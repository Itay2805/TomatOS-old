[global pic_disable]
pic_disable:
    push ebp
    mov ebp, esp

    mov al, 0xff
    out 0xa1, al
    out 0x21, al

    mov esp, ebp
    pop ebp
    ret
