[global paging_enable]
[global paging_disable]
[global paging_is_enabled]

paging_enable:
	push ebp
	mov ebp, esp

	mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
	
	mov esp, ebp
	pop ebp
	ret

paging_disable:
	push ebp
	mov ebp, esp

	mov eax, cr0 
    and eax, 0xFFFFFFFF7FFFFFFF
    mov cr0, eax

	mov esp,ebp
	pop ebp
	ret

paging_is_enabled:
	push ebp
	mov ebp, esp

	mov eax, cr0
	and eax, 0x80000000
	shr eax, 30

	mov esp, ebp
	pop ebp
	ret