extern main

[bits 64]
KernelStub:
	push rbp 
	mov rbp, rsp 

	mov rbp, 150000h
	mov rsp, rbp 
	call main
	
	hlt
	leave 
	ret