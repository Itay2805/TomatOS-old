global setjmp
global longjmp

setjmp:
	mov edx, [esp + 4]
	mov eax, [esp]
	mov [edx], eax
    mov [edx + 20], eax
    mov [edx + 0], ebp
    mov [edx + 4], ebx
    mov [edx + 8], edi
    mov [edx + 12], esi
    mov [edx + 16], esp
	xor eax, eax
	ret

longjmp:
	mov edx, [esp + 4]
    mov eax, [esp + 8]

    mov esp, [edx + 16]
    mov ebx, [edx + 20]
    mov [esp], ebx
    
    mov ebp, [edx + 0]
    mov ebx, [edx + 4]
    mov edi, [edx + 8]
    mov esi, [edx + 12]

	ret