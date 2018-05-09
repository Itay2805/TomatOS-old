[bits 32]
[extern kmain]
    call kmain
    hlt
kmain_exit:
	jmp kmain_exit