[global paging_enable]
[global paging_disable]
[global paging_is_enabled]

paging_enable:
	mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
	ret

paging_disable:
	mov eax, cr0 
    and eax, 0xFFFFFFFF7FFFFFFF
    mov cr0, eax
	ret

paging_is_enabled:
	mov eax, cr0
	and eax, 0x80000000
	shr eax, 30
	ret