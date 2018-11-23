[global kfault]
kfault:
	mov eax, 0
	lidt [eax]
	int 1