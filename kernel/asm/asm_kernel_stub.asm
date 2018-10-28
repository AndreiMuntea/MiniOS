extern main

[bits 64]
KernelStub:
    push rbp 
    mov rbp, rsp 

    call main
    
    hlt
    leave 
    ret